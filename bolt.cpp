#include <iostream>
#include <fstream>
#include <LibBolt/Object.h>
#include <LibBolt/Scaler.h>
#include <LibBolt/Symbol.h>
#include <LibBolt/List.h>
#include <LibBolt/Compiler.h>
#include <LibBolt/Instruction.h>
#include <LibBolt/Parser.h>

int main()
{
    // std::shared_ptr<Bolt::List> exp = std::make_shared<Bolt::List>(Bolt::List::Type::Expression);

    // std::shared_ptr<Bolt::Object> s1 = std::static_pointer_cast<Bolt::Object>(std::make_shared<Bolt::Scaler>(42));
    // std::shared_ptr<Bolt::Object> s2 = std::static_pointer_cast<Bolt::Object>(std::make_shared<Bolt::Scaler>(33));
    // std::shared_ptr<Bolt::Object> sym = std::static_pointer_cast<Bolt::Object>(std::make_shared<Bolt::Instruction>(Bolt::Instruction::Type::I_add));

    // exp->append(sym);
    // exp->append(s1);
    // exp->append(s2);

    // std::cout << exp.to_string() << '\n';
    // std::cout << exp.head()->to_string() << '\n';

    Bolt::Compiler compiler;
    // std::vector<std::shared_ptr<Bolt::Object>> code;
    // code.push_back(std::static_pointer_cast<Bolt::Object>(exp));



    Bolt::Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize("(add 2 3) (add 4 5)");
    Bolt::Parser parser;
    auto code = parser.parse(tokens);

    std::string out_code = compiler.compile(code);
    std::cout << out_code << '\n';

    return 0;
}
