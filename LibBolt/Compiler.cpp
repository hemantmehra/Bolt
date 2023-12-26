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

        for(auto obj: m_object_list) {
            // std::cout << obj->to_string() << '\n';

            if (obj->is_scaler()) {
                ss << "    ;; push SCALER" << '\n';
                ss << "    push " << SCALER_SHARED_PTR_CAST(obj)->as_integer() << '\n';
            }

            else if (obj->is_instruction()) {

                switch(INS_SHARED_PTR_CAST(obj)->type()) {

                case Instruction::Type::I_add:
                {
                    ss << "    ;; ADD" << '\n';
                    ss << "    pop rax" << '\n';
                    ss << "    pop rbx" << '\n';
                    ss << "    add rax, rbx" << '\n';
                    ss << "    push rax" << '\n';
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
                }
            }

            // ss << '\n';
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

                else {
                    eval(obj);
                }
            }

            else {
                eval(obj);
            }

            // else if (obj->is_cons()) {
            //     OBJECT_PTR car = NODE_CAR(obj);
            //     if (car->is_symbol() && SYMBOL_PTR_CAST(car)->to_string() == "if") {
            //         int else_label_idx = generate_label_idx();
            //         int endif_label_idx = generate_label_idx();

            //         auto if_type = LISP::PrimitiveProcedure::Type::If;
            //         auto else_type = LISP::PrimitiveProcedure::Type::Else;
            //         auto endif_type = LISP::PrimitiveProcedure::Type::EndIf;

            //         auto p_if = MAKE_PRIMITVE_PROCEDURE2(if_type, else_label_idx);
            //         auto p_else = MAKE_PRIMITVE_PROCEDURE3(else_type, else_label_idx, endif_label_idx);
            //         auto p_endif = MAKE_PRIMITVE_PROCEDURE2(endif_type, endif_label_idx);

            //         auto exp = NODE_CAR(NODE_CDR(obj));
            //         auto if_block = NODE_CAR(NODE_CDR(NODE_CDR(obj)));
            //         auto else_block = NODE_CAR(NODE_CDR(NODE_CDR(NODE_CDR(obj))));

            //         eval(exp, env);
            //         m_object_list.push_back(OBJECT_PTR_CAST(p_if));
            //         eval(if_block, env);
            //         m_object_list.push_back(OBJECT_PTR_CAST(p_else));
            //         eval(else_block, env);
            //         m_object_list.push_back(OBJECT_PTR_CAST(p_endif));
            //     }
            // }

            // else {
            //     CONS_PTR cons_obj = CONS_PTR_CAST(obj);
            //     OBJECT_PTR proc = cons_obj->as_car();
            //     OBJECT_PTR args = cons_obj->as_cdr();
            //     eval_list(args, env);
            //     eval(proc, env);
            //     return;
            // }

            // it++;
            i++;
        }
    }
}
