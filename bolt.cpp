#include <iostream>
#include <fstream>
#include <LibBolt/Object.h>
#include <LibBolt/Scaler.h>
#include <LibBolt/Symbol.h>
#include <LibBolt/Expression.h>
#include <LibBolt/Compiler.h>

int main()
{
    // Bolt::Scaler s1(42);
    // Bolt::Scaler s1(33);
    // Bolt::Symbol sym("add");
    Bolt::Expression exp;

    std::shared_ptr<Bolt::Object> s1 = std::static_pointer_cast<Bolt::Object>(std::make_shared<Bolt::Scaler>(42));
    std::shared_ptr<Bolt::Object> s2 = std::static_pointer_cast<Bolt::Object>(std::make_shared<Bolt::Scaler>(33));
    std::shared_ptr<Bolt::Object> sym = std::static_pointer_cast<Bolt::Object>(std::make_shared<Bolt::Symbol>("add"));

    exp.append(sym);
    exp.append(s1);
    exp.append(s2);

    // std::cout << exp.to_string() << '\n';
    // std::cout << exp.head()->to_string() << '\n';

    Bolt::Compiler compiler;

    std::string out_code = compiler.compile(s1);
    std::cout << out_code << '\n';

    return 0;
}
