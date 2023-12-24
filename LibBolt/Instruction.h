#ifndef PRIMITIVE_PROCEDURE_H
#define PRIMITIVE_PROCEDURE_H

#include <string>
#include <memory>
#include <LibBolt/Object.h>

namespace Bolt {
    class Instruction : public Object
    {
    public:
        enum class Type
        {
            K_If,
            K_Else,
            K_EndIf,
            OP_ADD
        };

        Instruction(Type);
        Instruction(Type, int);
        Instruction(Type, int, int);
        Type type();
        virtual bool is_instruction() const override { return true; }
        std::string to_string() override;

        int data_1() { return m_data_1; }
        int data_2() { return m_data_2; }

    private:
        Type m_type;
        int m_data_1;
        int m_data_2;
    };
}

#endif
