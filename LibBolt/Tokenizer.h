#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <memory>
#include <vector>
#include <LibBolt/Instruction.h>

namespace Bolt {
    class Token
    {
    public:
        enum class Type
        {
            Begin_Exp,
            End_Exp,
            Begin_Block,
            End_Block,
            Symbol,
            Scaler,
            Instruction,
        };

        Token(Type);
        Token(Type, int);
        Token(Type, std::string);
        Token(Type, Instruction::Type);
        Type type() { return m_type; } 
        int as_scaler_value();
        std::string as_symbol_value();
        Instruction::Type as_insturction_type();
        std::string to_string();
    
    private:
        Type m_type;
        int m_scaler_value;
        std::string m_string_value;
        Instruction::Type m_instruction_type;
    };

    class Tokenizer
    {
    public:
        std::vector<Token> tokenize(std::string);
        std::vector<std::string> split_to_words(std::string code);
        bool is_number(const std::string);
    };
}

#endif
