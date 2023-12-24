#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <vector>
#include <memory>
#include <LibBolt/Object.h>

namespace Bolt {
    class Expression : public Object
    {
    public:
        Expression();
        std::shared_ptr<Object> head();
        std::shared_ptr<Object> get(size_t);
        size_t length() { return m_obj_list.size(); }
        void append(std::shared_ptr<Object>);
        virtual bool is_expression() const override { return true; }
        std::string to_string() override;

    private:
        std::vector<std::shared_ptr<Object>> m_obj_list;
    };
}

#endif
