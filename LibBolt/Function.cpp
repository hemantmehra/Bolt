#include <iostream>
#include <LibBolt/Function.h>

// #define FUNC_DEBUG

namespace Bolt {
    Function::Function()
    {
        m_curr_offset = 0;
    }

    void Function::set_offset(std::string sym, size_t sz)
    {
        m_sym_offset_map[sym] = m_curr_offset;
#ifdef FUNC_DEBUG
        std::cout << "Set offset: " << sym << ":" << m_curr_offset << ":" << sz << '\n';
#endif
        m_curr_offset += sz;
    }

    int Function::get_offset(std::string sym)
    {
        auto p = m_sym_offset_map.find(sym);
        if (p == m_sym_offset_map.end()) return -1;
#ifdef FUNC_DEBUG
        std::cout << "Get offset: " << sym << ": " <<  p->second << '\n';
#endif
        return p->second;
    }

    size_t Function::get_current_offset()
    {
        return m_curr_offset;
    }

    std::string Function::to_string()
    {
        return "[Function]";
    }
}
