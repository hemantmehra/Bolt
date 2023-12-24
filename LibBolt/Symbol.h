#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <LibBolt/Object.h>

namespace Bolt {
    class Symbol : public Object
    {
    public:
        Symbol(std::string x) : m_name(x) {}
        std::string name() const;
        bool is_symbol() const override { return true; }
        std::string to_string() override;

    private:
        std::string m_name;
    };
}

#endif
