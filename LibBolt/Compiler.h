#ifndef COMPILER_H
#define COMPILER_H

#include <memory>
#include <vector>
#include <LibBolt/Object.h>
#include <LibBolt/Scaler.h>
#include <LibBolt/Expression.h>

#define OBJECT_SHARED_PTR std::shared_ptr<Bolt::Object>
#define OBJECT_SHARED_PTR_CAST(x) std::static_pointer_cast<Object>(x)
#define SCALER_SHARED_PTR_CAST(x) std::static_pointer_cast<Scaler>(x)
#define EXP_SHARED_PTR_CAST(x) std::dynamic_pointer_cast<Expression>(x)

namespace Bolt {
    class Compiler
    {
    public:
        std::string compile(std::vector<std::shared_ptr<Object>>);
        Compiler() : m_label_idx(0) {}

    private:
        int generate_label_idx();
        void eval(std::shared_ptr<Object>);
        void compiler_to_objects(std::vector<std::shared_ptr<Object>>);

        std::vector<std::shared_ptr<Object>> m_object_list;
        int m_label_idx;
    };
}

#endif
