#include <iostream>
#include <fstream>
#include <LibBolt/Object.h>
#include <LibBolt/Scaler.h>
#include <LibBolt/Symbol.h>
#include <LibBolt/List.h>
#include <LibBolt/Compiler.h>
#include <LibBolt/Instruction.h>
#include <LibBolt/Parser.h>

#define MAIN_DEBUG

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: bolt program.bolt" << '\n';
        return 1;
    }

    std::string filename(argv[1]);
    std::string output_filename = "output.asm";
    std::ifstream ifs(filename);
    if (ifs.fail()) {
        std::cout << "File: \'" << filename << "\' does not exist" << '\n';
        return 1;
    }
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

#ifdef MAIN_DEBUG
    std::cout << "[Read] " << filename << "\nFile length " << content.length() << '\n';
#endif

    ifs.close();

    Bolt::Tokenizer tokenizer;
    Bolt::Parser parser;
    Bolt::Compiler compiler;

    auto tokens = tokenizer.tokenize(content);
#ifdef MAIN_DEBUG
    std::cout << "[Tokenize] Number of tokens: " << tokens.size() << '\n';
#endif

    auto code = parser.parse(tokens);
#ifdef MAIN_DEBUG
    std::cout << "[Parse] " << '\n';
#endif

    std::string out_code = compiler.compile(code);
#ifdef MAIN_DEBUG
    std::cout << "[Compile] " << '\n';
#endif

    std::ofstream out_asm(output_filename);
#ifdef MAIN_DEBUG
    std::cout << "[Write] " << output_filename << '\n';
#endif

    out_asm << out_code;
    out_asm.close();

    return 0;
}
