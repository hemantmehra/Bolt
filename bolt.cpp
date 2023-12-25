#include <iostream>
#include <fstream>
#include <LibBolt/Object.h>
#include <LibBolt/Scaler.h>
#include <LibBolt/Symbol.h>
#include <LibBolt/Expression.h>
#include <LibBolt/Compiler.h>
#include <LibBolt/Instruction.h>
#include <LibBolt/Parser.h>

int main()
{
    std::shared_ptr<Bolt::Expression> exp = std::make_shared<Bolt::Expression>();

    std::shared_ptr<Bolt::Object> s1 = std::static_pointer_cast<Bolt::Object>(std::make_shared<Bolt::Scaler>(42));
    std::shared_ptr<Bolt::Object> s2 = std::static_pointer_cast<Bolt::Object>(std::make_shared<Bolt::Scaler>(33));
    std::shared_ptr<Bolt::Object> sym = std::static_pointer_cast<Bolt::Object>(std::make_shared<Bolt::Instruction>(Bolt::Instruction::Type::OP_ADD));

    exp->append(sym);
    exp->append(s1);
    exp->append(s2);

    // std::cout << exp.to_string() << '\n';
    // std::cout << exp.head()->to_string() << '\n';

    Bolt::Compiler compiler;
    std::vector<std::shared_ptr<Bolt::Object>> code;
    code.push_back(std::static_pointer_cast<Bolt::Object>(exp));

    // std::string out_code = compiler.compile(code);
    // std::cout << out_code << '\n';

    Bolt::Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize("(defun func)");
    Bolt::Parser parser;
    parser.parse(tokens);
    return 0;
}
