#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include <string>

namespace Bolt {
    class Object
    {
    public:
        virtual ~Object() = default;
        virtual bool is_nil() const { return false; }
        virtual bool is_scaler() const { return false; }
        virtual bool is_symbol() const { return false; }
        virtual bool is_list() const { return false; }
        virtual bool is_block() const { return false; }
        virtual bool is_instruction() const { return false; }

        virtual std::string to_string() = 0;
    };
}

#endif
