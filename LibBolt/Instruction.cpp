#include <LibBolt/Instruction.h>

namespace Bolt {
    Instruction::Type Instruction::convert_to_instruction_type(std::string s)
    {
        if (table.find(s) != table.end()) return table[s];
        return Instruction::Type::_INVALID;
    }

    Instruction::Instruction(Instruction::Type type)
        : m_type(type) {}
    
    Instruction::Instruction(Instruction::Type type, int data)
        : m_type(type), m_data_1(data), m_data_str_1("") {}
    
    Instruction::Instruction(Instruction::Type type, int data1, int data2)
        : m_type(type), m_data_1(data1), m_data_2(data2), m_data_str_1("") {}
    
    Instruction::Instruction(Instruction::Type type, int data1, int data2, std::string s)
        : m_type(type), m_data_1(data1), m_data_2(data2), m_data_str_1(s) {}
    
    Instruction::Instruction(Instruction::Type type, int data1, int data2, std::string s1, std::string s2)
        : m_type(type), m_data_1(data1), m_data_2(data2), m_data_str_1(s1), m_data_str_2(s2) {}

    Instruction::Type Instruction::type() { return m_type; }

    std::string Instruction::to_string()
    {
        return "[Instruction]";
    }
}
