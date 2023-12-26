#ifndef LIST_H
#define LIST_H

#include <string>
#include <vector>
#include <memory>
#include <LibBolt/Object.h>

namespace Bolt {
    class List : public Object
    {
    public:
        enum class Type
        {
            Block,
            Expression
        };

        List(Type);
        std::shared_ptr<Object> head();
        std::shared_ptr<Object> rest();
        std::shared_ptr<Object> get(size_t);
        size_t length() { return m_obj_list.size(); }
        void append(std::shared_ptr<Object>);
        virtual bool is_list() const override { return true; }
        virtual bool is_expression() const override { return m_type == Type::Expression; }
        virtual bool is_block() const override { return m_type == Type::Block; }
        std::string to_string() override;

    private:
        std::vector<std::shared_ptr<Object>> m_obj_list;
        Type m_type;
    };
}

#endif
