#include <iostream>
#include <LibBolt/Tokenizer.h>

// #define TOKENIZER_DEBUG

namespace Bolt {
    Token::Token(Type type) : m_type(type) {}

    Token::Token(Type type, int x) : m_type(type), m_scaler_value(x) {}

    Token::Token(Type type, std::string x) : m_type(type), m_string_value(x) {}

    Token::Token(Type type, Instruction::Type instruction_type) : m_type(type), m_instruction_type(instruction_type) {}

    int Token::as_scaler_value() { return m_scaler_value; }

    std::string Token::as_symbol_value() { return m_string_value; }

    std::string Token::as_string_value() { return m_string_value; }

    Instruction::Type Token::as_insturction_type() { return m_instruction_type; }

    std::string Token::to_string()
    {
        switch (m_type)
        {
        case Type::Begin_Exp:
            return "[Token] Begin_Exp";
            break;
        case Type::End_Exp:
            return "[Token] End_Exp";
            break;
        case Type::Begin_Block:
            return "[Token] Begin_Block";
            break;
        case Type::End_Block:
            return "[Token] End_Block";
            break;
        case Type::Symbol:
            return "[Token] " + as_symbol_value();
            break;
        case Type::Scaler:
            return "[Token] " + std::to_string(as_scaler_value());
            break;
        case Type::Instruction:
            return "[Token] instruction";
            break;
        case Type::String:
            return "[Token] \"" + as_symbol_value() + "\"";
            break;
        default:
            break;
        }
        return "";
    }

    std::vector<std::string> Tokenizer::split_to_words(std::string code)
    {
        std::vector<std::string> words;
        std::string current_word = "";
        bool inside_str = false;

        auto it = code.begin();
        while (it != code.end())
        {
            if (*it == '\"' && inside_str == false) {
                inside_str = true;
                if (current_word.length() > 0) words.push_back(current_word);
                words.push_back("str\"");
                current_word = "";
            }

            else if (*it == '\"' && inside_str == true) {
                inside_str = false;
                words.push_back(current_word);
                words.push_back("\"str");
                current_word = "";
            }

            else if (inside_str) {
                current_word += *it;
            }

            else if (std::isspace(*it) && current_word.length() != 0) {
                words.push_back(current_word);
                current_word = "";
            }

            else if (*it == '(') {
                if (current_word.length() > 0) words.push_back(current_word);
                words.push_back("(");
                current_word = "";
            }

            else if (*it == ')') {
                if (current_word.length() > 0) words.push_back(current_word);
                words.push_back(")");
                current_word = "";
            }

            else if (*it == '{') {
                if (current_word.length() > 0) words.push_back(current_word);
                words.push_back("{");
                current_word = "";
            }

            else if (*it == '}') {
                if (current_word.length() > 0) words.push_back(current_word);
                words.push_back("}");
                current_word = "";
            }

            else if (std::isspace(*it)) {}

            else {
                current_word += *it;
            }

            it++;
        }

        if (current_word.length() > 0)
            words.push_back(current_word);

        return words;
    }

    bool Tokenizer::is_number(const std::string s)
    {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it)) ++it;
        return !s.empty() && it == s.end();
    }

    std::vector<Token> Tokenizer::tokenize(std::string code)
    {
        std::vector<Token> tokens;
        std::vector<std::string> words = split_to_words(code);
        bool inside_str = false;

        for(auto it: words) {
            if (it == "str\"") {
                inside_str = true;
                continue;
            }

            else if (it == "\"str") {
                inside_str = false;
                continue;
            }

            else if (inside_str) {
                tokens.push_back(Token(Token::Type::String, it));
            }

            else if (it == "(") {
                tokens.push_back(Token(Token::Type::Begin_Exp));
            }

            else if (it == ")") {
                tokens.push_back(Token(Token::Type::End_Exp));
            }

            else if (it == "{") {
                tokens.push_back(Token(Token::Type::Begin_Block));
            }

            else if (it == "}") {
                tokens.push_back(Token(Token::Type::End_Block));
            }

            else if (is_number(it)) {
                tokens.push_back(Token(Token::Type::Scaler, stoi(it)));
            }

            else if (Instruction::convert_to_instruction_type("I_" + it) != Instruction::Type::_INVALID) {
                auto instruction_type = Instruction::convert_to_instruction_type("I_" + it);
                tokens.push_back(Token(Token::Type::Instruction, instruction_type));
            }

            else {
                tokens.push_back(Token(Token::Type::Symbol, it));
            }
        }

#ifdef TOKENIZER_DEBUG
            for (auto i : tokens) {
                std::cout << i.to_string() << '\n';
            }
#endif
        return tokens;
    }
}
