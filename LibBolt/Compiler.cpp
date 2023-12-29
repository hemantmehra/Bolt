#include <iostream>
#include <sstream>
#include <LibBolt/Assertion.h>
#include <LibBolt/Compiler.h>

// #define COMPILER_DEBUG

namespace Bolt {
    int Compiler::generate_label_idx()
    {
        return m_label_idx++;
    }

    std::string Compiler::compile(std::shared_ptr<Object> obj_list)
    {
        m_label_idx = 0;
        std::stringstream ss;
        m_object_list.clear();
        compile_to_objects(obj_list);

        ss << "segment .text" << '\n';
        ss << "global _start" << '\n';
        ss << "_start:" << '\n';
        ss << "    push rbp" << '\n';
        ss << "    mov rbp, rsp" << '\n';
        ss << "    sub rsp, " << m_stack_offset << '\n'; 

        for(auto obj: m_object_list) {
            // std::cout << obj->to_string() << '\n';

            if (obj->is_scaler()) {
                ss << "    ;; push SCALER" << '\n';
                ss << "    push " << SCALER_SHARED_PTR_CAST(obj)->as_integer() << '\n';
            }

            else if (obj->is_symbol()) {
                std::string s = SYM_SHARED_PTR_CAST(obj)->to_string();
                int stack_offset = m_symbol_stack_offset_map[s];

                ss << "    ;; push SYMBOL" << '\n';
                ss << "    mov rax, QWORD [rbp-" << stack_offset << "]" << '\n';
                ss << "    push rax" << '\n';
            }

            else if (obj->is_instruction()) {

                switch(INS_SHARED_PTR_CAST(obj)->type()) {
                
                case Instruction::Type::I_let:
                {
                    int offset = INS_SHARED_PTR_CAST(obj)->data_1();

                    ss << "    ;; Init Symbol" << '\n';
                    ss << "    pop rax" << '\n';
                    ss << "    mov QWORD [rbp-" << offset << "], rax" << '\n';
                    break;
                }

                case Instruction::Type::I_inc:
                {
                    int offset = INS_SHARED_PTR_CAST(obj)->data_1();

                    ss << "    ;; Inc" << '\n';
                    ss << "    add QWORD [rbp-" << offset << "], 1" << '\n';
                    break;
                }

                case Instruction::Type::I_dec:
                {
                    int offset = INS_SHARED_PTR_CAST(obj)->data_1();

                    ss << "    ;; Dec" << '\n';
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

                case Instruction::Type::I_label:
                {
                    
                    std::string label = "label_" + std::to_string(INS_SHARED_PTR_CAST(obj)->data_1());

                    ss << "    ;; label" << '\n';
                    ss << label << ":" << '\n';
                    break;
                }

                case Instruction::Type::I_jump:
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

                case Instruction::Type::I_endIf:
                {
                    
                    std::string endif_label_name = "label_" + std::to_string(INS_SHARED_PTR_CAST(obj)->data_1());

                    ss << "    ;; End If" << '\n';
                    ss << endif_label_name << ":" << '\n';
                    break;
                }

                case Instruction::Type::I_drop:
                {
                    ss << "    ;; Drop" << '\n';
                    ss << "    pop rax" << '\n';
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

        // EXIT asm code
        ss << "    ;; EXIT" << '\n';
        ss << "    mov rax, 60" << '\n';
        ss << "    pop rdi" << '\n';
        ss << "    syscall" << '\n';

        ss << '\n';

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
                    auto ins_endif = MAKE_INS2(Instruction::Type::I_endIf, endif_label_idx);

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

                    auto ins_jump = MAKE_INS2(Instruction::Type::I_jump, while_cond_label_idx);
                    auto ins_label_body = MAKE_INS2(Instruction::Type::I_label, while_body_label_idx);
                    auto ins_label_cond = MAKE_INS2(Instruction::Type::I_label, while_cond_label_idx);
                    auto ins_while = MAKE_INS2(Instruction::Type::I_while, while_body_label_idx);

                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_jump));
                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_label_body));
                    compile_to_objects(list->get(i+1));
                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_label_cond));
                    compile_to_objects(LIST_SHARED_PTR_CAST(obj)->rest());
                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_while));

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
                    std::cout << symbol->to_string() << '\n';
                    std::cout << scaler->to_string() << '\n';
#endif

                    std::string s = SYM_SHARED_PTR_CAST(symbol)->to_string();
                    int offset = m_stack_offset + 4;

                    auto ins_let = MAKE_INS2(Instruction::Type::I_let, offset);
                    m_symbol_stack_offset_map[s] = offset;
                    m_stack_offset += 8;
                    m_object_list.push_back(scaler);
                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_let));
#ifdef COMPILER_DEBUG
                    std::cout << "New Stack offset " << m_stack_offset << '\n';
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
                    int offset = m_symbol_stack_offset_map[s];

                    auto ins_inc = MAKE_INS2(ins_type, offset);
                    m_object_list.push_back(OBJECT_SHARED_PTR_CAST(ins_inc));
#ifdef COMPILER_DEBUG
                    std::cout << "New Stack offset " << m_stack_offset << '\n';
#endif
                }

                else {
                    eval(obj);
                }
            }

            else {
                eval(obj);
            }

            // auto ins_drop = MAKE_INS1(Instruction::Type::I_drop);
            // m_object_list.push_back(ins_drop);
            i++;
        }
    }
}
