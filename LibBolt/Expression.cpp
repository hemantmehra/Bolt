#include <sstream>
#include <LibBolt/Assertion.h>
#include <LibBolt/Expression.h>

namespace Bolt {
    Expression::Expression() {}

    std::shared_ptr<Object> Expression::head()
    {
        CHECK(m_obj_list.size() > 0);
        return m_obj_list[0];
    }

    std::shared_ptr<Object> Expression::get(size_t index)
    {
        CHECK(m_obj_list.size() > (index + 1));
        return m_obj_list[index];
    }

    void Expression::append(std::shared_ptr<Object> obj)
    {
        m_obj_list.push_back(obj);
    }

    std::string Expression::to_string()
    {
        std::stringstream ss;
        ss << "(";
        for (auto obj: m_obj_list) {
            ss << obj->to_string() << ' ';
        }
        ss << ")";

        return ss.str();
    }
}
