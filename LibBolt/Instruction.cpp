#include <LibBolt/Instruction.h>

namespace Bolt {
    Instruction::Instruction(Instruction::Type type)
        : m_type(type) {}
    
    Instruction::Instruction(Instruction::Type type, int data)
        : m_type(type), m_data_1(data) {}
    
    Instruction::Instruction(Instruction::Type type, int data1, int data2)
        : m_type(type), m_data_1(data1), m_data_2(data2) {}

    Instruction::Type Instruction::type() { return m_type; }

    std::string Instruction::to_string()
    {
        return "[Instruction]";
    }
}
