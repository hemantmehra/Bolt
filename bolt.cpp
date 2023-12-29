#include <iostream>
#include <fstream>
#include <LibBolt/Object.h>
#include <LibBolt/Scaler.h>
#include <LibBolt/Symbol.h>
#include <LibBolt/List.h>
#include <LibBolt/Compiler.h>
#include <LibBolt/Instruction.h>
#include <LibBolt/Parser.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: bolt program.bolt" << '\n';
        return 1;
    }

    std::string filename(argv[1]);
    std::ifstream ifs(filename);
    if (ifs.fail()) {
        std::cout << "File: \'" << filename << "\' does not exist" << '\n';
        return 1;
    }
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    Bolt::Tokenizer tokenizer;
    Bolt::Parser parser;
    Bolt::Compiler compiler;

    auto tokens = tokenizer.tokenize(content);
    auto code = parser.parse(tokens);
    std::string out_code = compiler.compile(code);
    std::cout << out_code << '\n';

    return 0;
}
