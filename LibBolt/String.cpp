#include <LibBolt/String.h>

namespace Bolt {
    std::string String::name() const
    {
        return m_name;
    }

    std::string String::to_string()
    {
        return m_name;
    }
}
