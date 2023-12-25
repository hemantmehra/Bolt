#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <memory>
#include <LibBolt/Object.h>

namespace Bolt {
    class Block : public Object
    {
    public:
        Block();
        std::shared_ptr<Object> get(size_t);
        size_t length() { return m_obj_list.size(); }
        void append(std::shared_ptr<Object>);
        virtual bool is_block() const override { return true; }
        std::string to_string() override;

    private:
        std::vector<std::shared_ptr<Object>> m_obj_list;
    };
}

#endif
