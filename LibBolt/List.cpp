#include <sstream>
#include <LibBolt/Assertion.h>
#include <LibBolt/List.h>

namespace Bolt {
    List::List(List::Type type) : m_type(type) {}

    std::shared_ptr<Object> List::head()
    {
        CHECK(m_obj_list.size() > 0);
        return m_obj_list[0];
    }

    std::shared_ptr<Object> List::rest()
    {
        CHECK(m_obj_list.size() > 1);
        auto new_l = std::make_shared<List>(List::Type::Expression);

        for (int i = 1; i < this->length(); i++) {
            new_l->append(this->get(i));
        }

        return std::static_pointer_cast<Object>(new_l);
    }

    std::shared_ptr<Object> List::get(size_t index)
    {
        CHECK(m_obj_list.size() > index);
        return m_obj_list[index];
    }

    void List::append(std::shared_ptr<Object> obj)
    {
        m_obj_list.push_back(obj);
    }

    std::string List::to_string()
    {
        std::stringstream ss;

        if (m_type == Type::Expression)
            ss << "(";
        else
            ss << "{\n";
        
        for (auto obj: m_obj_list) {
            ss << obj->to_string() << ' ';
        }

        if (m_type == Type::Expression)
            ss << ")";
        else
            ss << "\n}";

        return ss.str();
    }
}
