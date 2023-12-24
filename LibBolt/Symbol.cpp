#include <LibBolt/Symbol.h>

namespace Bolt {
    std::string Symbol::name() const
    {
        return m_name;
    }

    std::string Symbol::to_string()
    {
        return m_name;
    }
}
