#include <sstream>
#include <LibBolt/Assertion.h>
#include <LibBolt/Block.h>

namespace Bolt {
    Block::Block() {}

    std::shared_ptr<Object> Block::get(size_t index)
    {
        CHECK(m_obj_list.size() > index);
        return m_obj_list[index];
    }

    void Block::append(std::shared_ptr<Object> obj)
    {
        m_obj_list.push_back(obj);
    }

    std::string Block::to_string()
    {
        std::stringstream ss;
        ss << "{";
        for (auto obj: m_obj_list) {
            ss << obj->to_string() << ' ';
        }
        ss << "}";

        return ss.str();
    }
}
