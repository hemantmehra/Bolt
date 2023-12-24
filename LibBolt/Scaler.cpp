#include <string>
#include <LibBolt/Assertion.h>
#include <LibBolt/Scaler.h>

namespace Bolt {
    Scaler::Type Scaler::type() const
    {
        return m_type;
    }

    int Scaler::as_integer() const
    {
        return m_data.as_integer;
    }

    float Scaler::as_float() const
    {
        return m_data.as_float;
    }

    bool Scaler::as_boolean() const
    {
        return m_data.as_boolean;
    }

    char Scaler::as_char() const
    {
        return m_data.as_char;
    }

    Scaler::Scaler() : m_type(Type::Nil) {}
    
    Scaler::Scaler(int x) : m_type(Type::Integer)
    {
        m_data.as_integer = x;
    }

    Scaler::Scaler(float x) : m_type(Type::Float)
    {
        m_data.as_float = x;
    }

    Scaler::Scaler(bool x) : m_type(Type::Boolean)
    {
        m_data.as_boolean = x;
    }

    Scaler::Scaler(char x) : m_type(Type::Char)
    {
        m_data.as_char = x;
    }

    std::string Scaler::to_string()
    {
        switch (m_type)
        {
        case Scaler::Type::Integer:
            return std::to_string(m_data.as_integer);

        case Scaler::Type::Float:
            return std::to_string(m_data.as_float);

        case Scaler::Type::Boolean:
            return std::to_string(m_data.as_boolean);

        case Scaler::Type::Char:
            return std::to_string(m_data.as_char);

        case Scaler::Type::Nil:
            return "nil";
        
        default:
            ASSERT_NOT_REACHED();
            break;
        }

        return "";
    }
}
