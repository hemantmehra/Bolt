#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <memory>
#include <unordered_map>
#include <LibBolt/Object.h>

#define ENUMERATE_INSTRUCTION_TYPES \
    __ENUMERATE_INSTRUCTION_TYPE(_INVALID) \
    __ENUMERATE_INSTRUCTION_TYPE(I_if) \
    __ENUMERATE_INSTRUCTION_TYPE(I_else) \
    __ENUMERATE_INSTRUCTION_TYPE(__endIf) \
    __ENUMERATE_INSTRUCTION_TYPE(I_add) \
    __ENUMERATE_INSTRUCTION_TYPE(I_let) \
    __ENUMERATE_INSTRUCTION_TYPE(I_global) \
    __ENUMERATE_INSTRUCTION_TYPE(__drop) \
    __ENUMERATE_INSTRUCTION_TYPE(I_inc) \
    __ENUMERATE_INSTRUCTION_TYPE(I_dec) \
    __ENUMERATE_INSTRUCTION_TYPE(I_eq) \
    __ENUMERATE_INSTRUCTION_TYPE(I_lt) \
    __ENUMERATE_INSTRUCTION_TYPE(I_gt) \
    __ENUMERATE_INSTRUCTION_TYPE(I_while) \
    __ENUMERATE_INSTRUCTION_TYPE(__jump) \
    __ENUMERATE_INSTRUCTION_TYPE(__label) \
    __ENUMERATE_INSTRUCTION_TYPE(I_print) \
    __ENUMERATE_INSTRUCTION_TYPE(I_exit) \
    __ENUMERATE_INSTRUCTION_TYPE(I_defun) \
    __ENUMERATE_INSTRUCTION_TYPE(__call) \
    __ENUMERATE_INSTRUCTION_TYPE(__func_start) \
    __ENUMERATE_INSTRUCTION_TYPE(__func_end) \
    __ENUMERATE_INSTRUCTION_TYPE(__prog_start) \
    __ENUMERATE_INSTRUCTION_TYPE(__prog_end) \
    __ENUMERATE_INSTRUCTION_TYPE(I_set)

namespace Bolt {
    class Instruction : public Object
    {
    public:
        enum class Type
        {
#define __ENUMERATE_INSTRUCTION_TYPE(x) x,
        ENUMERATE_INSTRUCTION_TYPES
#undef __ENUMERATE_INSTRUCTION_TYPE
        };

        Instruction(Type);
        Instruction(Type, int);
        Instruction(Type, int, int);
        Instruction(Type, int, int, std::string);
        Type type();
        virtual bool is_instruction() const override { return true; }
        std::string to_string() override;

        int data_1() { return m_data_1; }
        int data_2() { return m_data_2; }
        std::string data_str() { return m_data_str; }

        inline static std::unordered_map<std::string, Type> table = {
#define __ENUMERATE_INSTRUCTION_TYPE(x) {#x, Type::x},
        ENUMERATE_INSTRUCTION_TYPES
#undef __ENUMERATE_INSTRUCTION_TYPE            
        };

        static Type convert_to_instruction_type(std::string);

    private:
        Type m_type;
        int m_data_1;
        int m_data_2;
        std::string m_data_str;
    };
}

#endif
