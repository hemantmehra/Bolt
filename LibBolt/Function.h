#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <LibBolt/Object.h>

namespace Bolt {
    class Function : public Object
    {
    public:
        Function();
        void set_offset(std::string, size_t);
        int get_offset(std::string);
        size_t get_current_offset();

        virtual bool is_function() const override { return true; }
        std::string to_string() override;

    private:
        std::unordered_map<std::string, int> m_sym_offset_map;
        size_t m_curr_offset;
    };
}

#endif
