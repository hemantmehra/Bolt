#include <iostream>
#include <LibBolt/Assertion.h>
#include <LibBolt/Parser.h>
#include <LibBolt/Scaler.h>
#include <LibBolt/Symbol.h>
#include <LibBolt/List.h>

// #define PARSER_DEBUG

#define MAKE_EXP() std::make_shared<List>(List::Type::Expression)
#define MAKE_BLOCK() std::make_shared<List>(List::Type::Block)
#define OBJECT_SHARED_PTR_CAST(x) std::static_pointer_cast<Object>(x)
#define MAKE_SCALER(x) std::make_shared<Scaler>(x)
#define MAKE_SYMBOL(x) std::make_shared<Symbol>(x)

namespace Bolt {
    std::shared_ptr<Object> Parser::parse(std::vector<Token> tokens)
    {
        std::shared_ptr<List> ptr = MAKE_BLOCK();
        std::vector<std::shared_ptr<List>> stack;

        for (auto curr_token: tokens)
        {
#ifdef PARSER_DEBUG
            std::cout << curr_token.to_string() << '\n';
#endif
            switch (curr_token.type())
            {
            case Token::Type::Begin_Block:
            {
                stack.push_back(ptr);
                ptr = MAKE_BLOCK();
                break;
            }

            case Token::Type::End_Block:
            {
                std::shared_ptr<List> tmp1 = stack.back();
                tmp1->append(ptr);
                ptr = tmp1;
                stack.pop_back();
                break;
            }

            case Token::Type::Begin_Exp:
            {
                stack.push_back(ptr);
                ptr = MAKE_EXP();
                break;
            }

            case Token::Type::End_Exp:
            {
                std::shared_ptr<List> tmp1 = stack.back();
                tmp1->append(ptr);
                ptr = tmp1;
                stack.pop_back();
                break;
            }

            case Token::Type::Symbol:
            {
                auto symbol_obj = OBJECT_SHARED_PTR_CAST(MAKE_SYMBOL(curr_token.as_symbol_value()));
                ptr->append(symbol_obj);
                break;
            }

            case Token::Type::Scaler:
            {
                auto scaler_obj = OBJECT_SHARED_PTR_CAST(MAKE_SCALER(curr_token.as_scaler_value()));
                ptr->append(scaler_obj);
                break;
            }
            
            default:
                break;
            }
#ifdef PARSER_DEBUG
            std::cout << ptr->to_string() << '\n';
#endif
        }

#ifdef PARSER_DEBUG
            std::cout << "Final Parse Tree:\n" << ptr->to_string() << '\n';
#endif
        return ptr;
    }
}
