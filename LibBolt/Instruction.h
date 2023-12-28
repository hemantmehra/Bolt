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
    __ENUMERATE_INSTRUCTION_TYPE(I_endIf) \
    __ENUMERATE_INSTRUCTION_TYPE(I_add) \
    __ENUMERATE_INSTRUCTION_TYPE(I_let) \
    __ENUMERATE_INSTRUCTION_TYPE(I_drop) \
    __ENUMERATE_INSTRUCTION_TYPE(I_inc) \
    __ENUMERATE_INSTRUCTION_TYPE(I_dec) \

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
        Type type();
        virtual bool is_instruction() const override { return true; }
        std::string to_string() override;

        int data_1() { return m_data_1; }
        int data_2() { return m_data_2; }

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
    };
}

#endif
