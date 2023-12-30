#include <iostream>
#include <sstream>
#include <algorithm>
#include <LibBolt/Assertion.h>
#include <LibBolt/Compiler.h>

// #define COMPILER_DEBUG

namespace Bolt {
    int Compiler::generate_label_idx()
    {
        return m_label_idx++;
    }

    int Compiler::get_symbol_stack_offset(std::string s)
    {
        return m_current_function->get_offset(s);
    }

    int Compiler::get_symbol_bss_offset(std::string s)
    {
        if(m_symbol_bss_offset_map.find(s) == m_symbol_bss_offset_map.end()) return -1;
        return m_symbol_bss_offset_map[s];
    }

    std::string Compiler::compile(std::shared_ptr<Object> obj_list)
    {
        m_label_idx = 0;
        std::stringstream ss;
        m_object_list.clear();
        
        compile_to_objects(obj_list);

        auto ins_prog_start = MAKE_INS2(Instruction::Type::__prog_start, 0);
        auto ins_prog_end = MAKE_INS2(Instruction::Type::__prog_end, 0);
        m_object_list.push_back(ins_prog_start);
        m_object_list.push_back(ins_prog_end);

        ss << "segment .bss" << '\n';
        ss << "bss_mem: resb " << m_bss_offset << '\n';
        ss << "segment .text" << '\n';
        ss << "global _start" << '\n';

        ss << "print:\n";                              
        ss << "    mov     r9, -3689348814741910323\n";
        ss << "    sub     rsp, 40\n";                 
        ss << "    mov     BYTE [rsp+31], 10\n";       
        ss << "    lea     rcx, [rsp+30]\n";           
        ss << ".L2:\n";                                
        ss << "    mov     rax, rdi\n";                
        ss << "    lea     r8, [rsp+32]\n";            
        ss << "    mul     r9\n";                      
        ss << "    mov     rax, rdi\n";                
        ss << "    sub     r8, rcx\n";                 
        ss << "    shr     rdx, 3\n";                  
        ss << "    lea     rsi, [rdx+rdx*4]\n";        
        ss << "    add     rsi, rsi\n";                
        ss << "    sub     rax, rsi\n";                
        ss << "    add     eax, 48\n";                 
        ss << "    mov     BYTE [rcx], al\n";          
        ss << "    mov     rax, rdi\n";                
        ss << "    mov     rdi, rdx\n";                
        ss << "    mov     rdx, rcx\n";                
        ss << "    sub     rcx, 1\n";                  
        ss << "    cmp     rax, 9\n";                  
        ss << "    ja      .L2\n";                     
        ss << "    lea     rax, [rsp+32]\n";           
        ss << "    mov     edi, 1\n";                  
        ss << "    sub     rdx, rax\n";                
        ss << "    xor     eax, eax\n";                
        ss << "    lea     rsi, [rsp+32+rdx]\n";       
        ss << "    mov     rdx, r8\n";                 
        ss << "    mov     rax, 1\n";                  
        ss << "    syscall\n";                         
        ss << "    add     rsp, 40\n";                 
        ss << "    ret\n";

        for(auto obj: m_object_list) {

#ifdef COMPILER_DEBUG
            std::cout << "{{ " << obj->to_string() << " }}" << '\n';
#endif
            if (obj->is_scaler()) {
                ss << "    ;; push SCALER" << '\n';
                ss << "    push " << SCALER_SHARED_PTR_CAST(obj)->as_integer() << '\n';
            }

            else if (obj->is_symbol()) {
                std::string s = SYM_SHARED_PTR_CAST(obj)->to_string();

                if (std::find(m_func_names.begin(), m_func_names.end(), s) != m_func_names.end())
                {
                    ss << "    ;; Function call" << '\n';
                    ss << "    call " << s << '\n';
                    continue;
                }

                int stack_offset = get_symbol_stack_offset(s);
                int bss_offset = get_symbol_bss_offset(s);
                if (stack_offset == -1 && bss_offset == -1) {
                    std::cerr << "Undefined variable: " << s << '\n';
                    std::cerr << "Current Function data" << "\n";
                    std::cerr << m_current_function->to_string();
                    exit(1);
                }

                ss << "    ;; push SYMBOL" << '\n';
                if (stack_offset != -1) {
                    ss << "    mov rax, QWORD [rbp-" << stack_offset << "]" << '\n';
                }
                else {
                    ss << "    mov rax, QWORD [bss_mem+" << bss_offset << "]" << '\n';
                }
                ss << "    push rax" << '\n';
            }

            else if (obj->is_function()) {
                m_current_function = std::dynamic_pointer_cast<Function>(obj);
            }
            
            else if (obj->is_instruction()) {

                switch(INS_SHARED_PTR_CAST(obj)->type()) {

                case Instruction::Type::__prog_start:
                {
                    ss << "_start:" << '\n';
                    ss << "    call main" << "\n";
                    break;
                }

                case Instruction::Type::__prog_end:
                {
                    ss << "    ;; EXIT" << '\n';
                    ss << "    mov rax, 60" << '\n';
                    ss << "    mov rdi, 0" << '\n';
                    ss << "    syscall" << '\n';
                    ss << ";; ----------" << '\n';
                    break;
                }

                case Instruction::Type::I_print:
                {
                    ss << "    ;; print" << '\n';
                    ss << "    pop rdi" << '\n';
                    ss << "    call print" << '\n';
                    break;
                }

                case Instruction::Type::I_exit:
                {
                    ss << "    ;; EXIT" << '\n';
                    ss << "    mov rax, 60" << '\n';
                    ss << "    pop rdi" << '\n';
                    ss << "    syscall" << '\n';
                    break;
                }
                
                case Instruction::Type::I_let:
                {
                    int offset = INS_SHARED_PTR_CAST(obj)->data_1();

                    ss << "    ;; Init Symbol" << '\n';
                    ss << "    pop rax" << '\n';
                    ss << "    mov QWORD [rbp-" << offset << "], rax" << '\n';
                    break;
                }

                case Instruction::Type::I_global:
                case Instruction::Type::I_defun:
                {
                    break;
                }

                case Instruction::Type::__call:
                {
                    NOT_IMPLEMENTED();
                    break;
                }

                case Instruction::Type::I_inc:
                {
                    int offset = INS_SHARED_PTR_CAST(obj)->data_1();
                    int offset_type = INS_SHARED_PTR_CAST(obj)->data_2();

                    ss << "    ;; Inc" << '\n';

                    if (offset_type)
                        ss << "    add QWORD [bss_mem+" << offset << "], 1" << '\n';
                    else
                        ss << "    add QWORD [rbp-" << offset << "], 1" << '\n';
                    break;
                }

                case Instruction::Type::I_dec:
                {
                    int offset = INS_SHARED_PTR_CAST(obj)->data_1();
                    int offset_type = INS_SHARED_PTR_CAST(obj)->data_2();

                    ss << "    ;; Dec" << '\n';

                    if (offset_type)
                        ss << "    sub QWORD [bss_mem+" << offset << "], 1" << '\n';
                    else
                        ss << "    sub QWORD [rbp-" << offset << "], 1" << '\n';
                    break;
                }

                case Instruction::Type::I_add:
                {
                    ss << "    ;; ADD" << '\n';
                    ss << "    pop rax" << '\n';
                    ss << "    pop rbx" << '\n';
                    ss << "    add rax, rbx" << '\n';
                    ss << "    push rax" << '\n';
                    break;
                }

                case Instruction::Type::I_eq:
                {
                    ss << "    ;; EQ" << '\n';
                    ss << "    mov rcx, 0" << '\n';
                    ss << "    mov rdx, 1" << '\n';
                    ss << "    pop rax" << '\n';
                    ss << "    pop rbx" << '\n';
                    ss << "    cmp rax, rbx" << '\n';
                    ss << "    cmove rcx, rdx" << '\n';
                    ss << "    push rcx" << '\n';
                    break;
                }

                case Instruction::Type::I_lt:
                {
                    ss << "    ;; LT" << '\n';
                    ss << "    mov rcx, 0" << '\n';
                    ss << "    mov rdx, 1" << '\n';
                    ss << "    pop rax" << '\n';
                    ss << "    pop rbx" << '\n';
                    ss << "    cmp rax, rbx" << '\n';
                    ss << "    cmovg rcx, rdx" << '\n';
                    ss << "    push rcx" << '\n';
                    break;
                }

                case Instruction::Type::I_gt:
                {
                    ss << "    ;; GT" << '\n';
                    ss << "    mov rcx, 0" << '\n';
                    ss << "    mov rdx, 1" << '\n';
                    ss << "    pop rax" << '\n';
                    ss << "    pop rbx" << '\n';
                    ss << "    cmp rax, rbx" << '\n';
                    ss << "    cmovl rcx, rdx" << '\n';
                    ss << "    push rcx" << '\n';
                    break;
                }

                case Instruction::Type::__label:
                {
                    std::string label = "label_" + std::to_string(INS_SHARED_PTR_CAST(obj)->data_1());
                    std::string label_name = INS_SHARED_PTR_CAST(obj)->data_str();

                    ss << "    ;; label" << '\n';

                    if (label_name.length())
                        ss << label_name << ":" << '\n';
                    else
                        ss << label << ":" << '\n';
                    break;
                }

                case Instruction::Type::__jump:
                {
                    
                    std::string label = "label_" + std::to_string(INS_SHARED_PTR_CAST(obj)->data_1());

                    ss << "    ;; jump" << '\n';
                    ss << "    " << "jmp " << label << '\n';
                    break;
                }

                case Instruction::Type::I_while:
                {
                    
                    std::string label = "label_" + std::to_string(INS_SHARED_PTR_CAST(obj)->data_1());

                    ss << "    ;; while" << '\n';
                    ss << "    pop rax" << '\n';
                    ss << "    test rax, rax" << '\n';
                    ss << "    jnz " << label << '\n';
                    break;
                }

                case Instruction::Type::I_if:
                {
                    
                    std::string else_label_name = "label_" + std::to_string(INS_SHARED_PTR_CAST(obj)->data_1());

                    ss << "    ;; If" << '\n';
                    ss << "    pop rax" << '\n';
                    ss << "    test rax, rax" << '\n';
                    ss << "    jz " << else_label_name << '\n';
                    break;
                }

                case Instruction::Type::I_else:
                {
                    
                    std::string else_label_name = "label_" + std::to_string(INS_SHARED_PTR_CAST(obj)->data_1());
                    std::string endif_label_name = "label_" + std::to_string(INS_SHARED_PTR_CAST(obj)->data_2());

                    ss << "    ;; Else" << '\n';
                    ss << "    jmp " << endif_label_name << '\n';
                    ss << else_label_name << ":" << '\n';
                    break;
                }

                case Instruction::Type::__endIf:
                {
                    
                    std::string endif_label_name = "label_" + std::to_string(INS_SHARED_PTR_CAST(obj)->data_1());

                    ss << "    ;; End If" << '\n';
                    ss << endif_label_name << ":" << '\n';
                    break;
                }

                case Instruction::Type::__drop:
                {
                    ss << "    ;; Drop" << '\n';
                    ss << "    pop rax" << '\n';
                    break;
                }

                case Instruction::Type::__func_start:
                {
                    size_t offset = m_current_function->get_current_offset();
                    ss << "    ;; Func Start" << '\n';
                    ss << "    push rbp" << '\n';
                    ss << "    mov rbp, rsp" << '\n';
                    ss << "    sub rsp, " << offset << "\n";
                    break;
                }

                case Instruction::Type::__func_end:
                {
                    size_t offset = m_current_function->get_current_offset();
                    ss << "    ;; Func End" << '\n';
                    ss << "    pop rbp" << '\n';
                    ss << "    add rsp, " << offset << "\n";
                    ss << "    ret" << '\n';
                    break;
                }

                case Instruction::Type::_INVALID:
                {
                    ASSERT_NOT_REACHED();
                    break;
                }
                }
            }

        }

        return ss.str();
    }

    void Compiler::eval(std::shared_ptr<Object> obj)
    {
        if (obj->is_scaler()) {
            m_object_list.push_back(obj);
            return;
        }

        else if (obj->is_symbol()) {
            m_object_list.push_back(obj);
            return;
        }

        else if (obj->is_list()) {
            auto exp = LIST_SHARED_PTR_CAST(obj);
            for (size_t i = 1; i < exp->length(); i++) {
                m_object_list.push_back(exp->get(i));
            }

            m_object_list.push_back(exp->head());
            return;
        }
    }

    void Compiler::compile_to_objects(std::shared_ptr<Object> obj_list)
    {
        auto list = std::dynamic_pointer_cast<List>(obj_list);
        size_t length = list->length();
        size_t i = 0;

        while (i < length) {
            auto obj = list->get(i);

#ifdef COMPILER_DEBUG
            std::cout << obj->to_string() << '\n';
#endif
            if (obj->is_expression() && LIST_SHARED_PTR_CAST(obj)->head()->is_instruction()) {
                auto head = LIST_SHARED_PTR_CAST(obj)->head();
                auto ins_type = INS_SHARED_PTR_CAST(head)->type();
                if (ins_type == Instruction::Type::I_if) {
                    int else_label_idx = generate_label_idx();
                    int endif_label_idx = generate_label_idx();

                    CHECK(i+3 < length);
                    auto if_body = list->get(i+1);
                    auto else_exp = list->get(i+2);
                    auto else_body = list->get(i+3);
                    CHECK(if_body->is_block());
                    CHECK(else_exp->is_expression());
                    CHECK(else_body->is_block());

                    auto ins_if = MAKE_INS2(Instruction::Type::I_if, else_label_idx);
                    auto ins_else = MAKE_INS3(Instruction::Type::I_else, else_label_idx, endif_label_idx);
                    auto ins_endif = MAKE_INS2(Instruction::Type::__endIf, endif_label_idx);

                    compile_to_objects(LIST_SHARED_PTR_CAST(obj)->rest());
                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_if));
                    compile_to_objects(list->get(i+1));
                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_else));
                    compile_to_objects(list->get(i+3));
                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_endif));

                    i += 3;
                }

                else if (ins_type == Instruction::Type::I_while) {
                    int while_cond_label_idx = generate_label_idx();
                    int while_body_label_idx = generate_label_idx();

                    CHECK(i+1 < length);
                    auto while_body = list->get(i+1);
                    CHECK(while_body->is_block());

                    auto ins_jump = MAKE_INS2(Instruction::Type::__jump, while_cond_label_idx);
                    auto ins_label_body = MAKE_INS2(Instruction::Type::__label, while_body_label_idx);
                    auto ins_label_cond = MAKE_INS2(Instruction::Type::__label, while_cond_label_idx);
                    auto ins_while = MAKE_INS2(Instruction::Type::I_while, while_body_label_idx);

                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_jump));
                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_label_body));
                    compile_to_objects(list->get(i+1));
                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_label_cond));
                    compile_to_objects(LIST_SHARED_PTR_CAST(obj)->rest());
                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_while));

                    i++;
                }

                else if (ins_type == Instruction::Type::I_defun) {
                    // int defun_label_idx = generate_label_idx();
                    m_current_function = std::make_shared<Function>();
                    m_object_list.push_back(std::static_pointer_cast<Object>(m_current_function));
                    
                    auto rest = LIST_SHARED_PTR_CAST(obj)->rest();
                    CHECK(LIST_SHARED_PTR_CAST(rest)->length() > 0);
                    auto symbol = LIST_SHARED_PTR_CAST(rest)->get(0);

                    CHECK(i+1 < length);

                    auto defun_body = list->get(i+1);
                    CHECK(defun_body->is_block());

                    m_func_names.push_back(symbol->to_string());
                    auto ins_label = MAKE_INS4(Instruction::Type::__label, 0, 0, symbol->to_string());
                    auto ins_func_start = MAKE_INS2(Instruction::Type::__func_start, 0);
                    auto ins_func_end = MAKE_INS2(Instruction::Type::__func_end, 0);

                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_label));
                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_func_start));
                    compile_to_objects(list->get(i+1));
                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_func_end));

                    i++;
                }

                else if (ins_type == Instruction::Type::I_let) {
                    auto rest = LIST_SHARED_PTR_CAST(obj)->rest();
                    CHECK(LIST_SHARED_PTR_CAST(rest)->length() > 1);

                    auto symbol = LIST_SHARED_PTR_CAST(rest)->get(0);
                    auto scaler = LIST_SHARED_PTR_CAST(rest)->get(1);

                    CHECK(symbol->is_symbol());
                    CHECK(scaler->is_scaler());
#ifdef COMPILER_DEBUG
                    std::cout << "Let Sym: "<< symbol->to_string() << '\n';
                    std::cout << "Let Scaler: "<< scaler->to_string() << '\n';
#endif

                    std::string s = SYM_SHARED_PTR_CAST(symbol)->to_string();
                    auto ins_let = MAKE_INS2(Instruction::Type::I_let, m_current_function->get_current_offset());
                    m_current_function->set_offset(s, 8);
#ifdef COMPILER_DEBUG
                    std::cout << m_current_function->to_string() << '\n';
#endif
                    m_object_list.push_back(scaler);
                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_let));
#ifdef COMPILER_DEBUG
                    std::cout << "New Stack offset " << m_stack_offset << '\n';
#endif
                }

                else if (ins_type == Instruction::Type::I_global) {
                    auto rest = LIST_SHARED_PTR_CAST(obj)->rest();
                    CHECK(LIST_SHARED_PTR_CAST(rest)->length() > 0);

                    auto symbol = LIST_SHARED_PTR_CAST(rest)->get(0);

                    CHECK(symbol->is_symbol());
#ifdef COMPILER_DEBUG
                    std::cout << symbol->to_string() << '\n';
#endif

                    std::string s = SYM_SHARED_PTR_CAST(symbol)->to_string();
                    auto ins_let = MAKE_INS2(Instruction::Type::I_global, m_bss_offset);
                    m_symbol_bss_offset_map[s] = m_bss_offset;
                    m_bss_offset += 8;
                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_let));
#ifdef COMPILER_DEBUG
                    std::cout << "New Stack offset " << m_bss_offset << '\n';
#endif
                }

                else if (ins_type == Instruction::Type::I_inc || ins_type == Instruction::Type::I_dec) {
                    auto rest = LIST_SHARED_PTR_CAST(obj)->rest();
                    CHECK(LIST_SHARED_PTR_CAST(rest)->length() > 0);

                    auto symbol = LIST_SHARED_PTR_CAST(rest)->get(0);

                    CHECK(symbol->is_symbol());
#ifdef COMPILER_DEBUG
                    std::cout << symbol->to_string() << '\n';
#endif

                    std::string s = SYM_SHARED_PTR_CAST(symbol)->to_string();
                    int offset = get_symbol_stack_offset(s);
                    int bss_offset = get_symbol_bss_offset(s);
                    if (offset == -1 && bss_offset == -1) {
                        std::cerr << "Undefined variable: " << s << '\n';
                        exit(1);
                    }

                    if (offset != -1) {
                        auto ins_inc = MAKE_INS3(ins_type, offset, 0);
                        m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_inc));
                    }

                    else {
                        auto ins_inc = MAKE_INS3(ins_type, bss_offset, 1);
                        m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_inc));
                    }
#ifdef COMPILER_DEBUG
                    std::cout << "New offset " << offset << '\n';
#endif
                }

                else {
                    eval(obj);
                }
            }

            else if (obj->is_expression()) {
                eval(LIST_SHARED_PTR_CAST(obj)->head());
            }

            else {
                eval(obj);
            }

            i++;
        }
    }
}
