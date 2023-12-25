#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <memory>
#include <vector>
#include <LibBolt/Tokenizer.h>
#include <LibBolt/Object.h>

namespace Bolt {
    class Parser
    {
    public:
        std::shared_ptr<Object> parse(std::vector<Token>);
    };
}

#endif
