#include <iostream>
#include <sstream>
#include <LibBolt/Compiler.h>

namespace Bolt {
    int Compiler::generate_label_idx()
    {
        return m_label_idx++;
    }

    std::string Compiler::compile(std::vector<std::shared_ptr<Object>> obj_list)
    {
        m_label_idx = 0;
        std::stringstream ss;
        m_object_list.clear();
        compiler_to_objects(obj_list);

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

    void Compiler::compiler_to_objects(std::vector<std::shared_ptr<Object>> obj_list)
    {
        auto it = obj_list.begin();
        bool compiled = false;

        while (it != obj_list.end()) {
            auto obj = *it;
            if (obj->is_list()) {
                auto exp = LIST_SHARED_PTR_CAST(obj);
                if (exp->head()->to_string() == "if") {
                    compiled = true;
                }
            }

            if (!compiled) eval(obj);

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

            it++;
        }
    }
}
