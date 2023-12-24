#ifndef SCALER_H
#define SCALER_H

#include <string>
#include <LibBolt/Object.h>

namespace Bolt {
    class Scaler : public Object
    {
    public:
        enum class Type
        {
            Integer,
            Float,
            Boolean,
            Char,
            Nil
        };

        Type type() const;
        int as_integer() const;
        float as_float() const;
        bool as_boolean() const;
        char as_char() const;

        Scaler();
        Scaler(int);
        Scaler(float);
        Scaler(bool);
        Scaler(char);

        virtual bool is_scaler() const override { return true; }
        virtual bool is_nil() const override { return m_type == Type::Nil; }
        std::string to_string() override;

    private:
        Type m_type;

        union {
            int as_integer;
            float as_float;
            bool as_boolean;
            char as_char;
        } m_data;
    };
}

#endif