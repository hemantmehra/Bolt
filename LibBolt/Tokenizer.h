#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <memory>
#include <vector>

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
            Scaler
        };

        Token(Type);
        Token(Type, int);
        Token(Type, std::string);
        Type type() { return m_type; } 
        int as_scaler_value();
        std::string as_symbol_value();
        std::string to_string();
    
    private:
        Type m_type;
        int m_scaler_value;
        std::string m_string_value;
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
