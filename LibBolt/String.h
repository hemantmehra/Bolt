#ifndef STRING_H
#define STRING_H

#include <string>
#include <LibBolt/Object.h>

namespace Bolt {
    class String : public Object
    {
    public:
        String(std::string x) : m_name(x) {}
        std::string name() const;
        bool is_string() const override { return true; }
        std::string to_string() override;

    private:
        std::string m_name;
    };
}

#endif
