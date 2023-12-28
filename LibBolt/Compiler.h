#ifndef COMPILER_H
#define COMPILER_H

#include <memory>
#include <vector>
#include <LibBolt/Object.h>
#include <LibBolt/Scaler.h>
#include <LibBolt/Symbol.h>
#include <LibBolt/List.h>
#include <LibBolt/Instruction.h>

#define OBJECT_SHARED_PTR std::shared_ptr<Bolt::Object>
#define MAKE_INS1(x) std::make_shared<Bolt::Instruction>(x)
#define MAKE_INS2(x, y) std::make_shared<Bolt::Instruction>(x, y)
#define MAKE_INS3(x, y, z) std::make_shared<Bolt::Instruction>(x, y, z)

#define OBJECT_SHARED_PTR_CAST(x) std::static_pointer_cast<Object>(x)
#define SCALER_SHARED_PTR_CAST(x) std::static_pointer_cast<Scaler>(x)
#define SYM_SHARED_PTR_CAST(x) std::dynamic_pointer_cast<Symbol>(x)
#define LIST_SHARED_PTR_CAST(x) std::dynamic_pointer_cast<List>(x)
#define INS_SHARED_PTR_CAST(x) std::dynamic_pointer_cast<Instruction>(x)

namespace Bolt {
    class Compiler
    {
    public:
        std::string compile(std::shared_ptr<Object>);
        Compiler() : m_label_idx(0), m_stack_offset(0) {}

    private:
        int generate_label_idx();
        void eval(std::shared_ptr<Object>);
        void compile_to_objects(std::shared_ptr<Object>);

        std::vector<std::shared_ptr<Object>> m_object_list;
        int m_label_idx;
        int m_stack_offset;

        std::unordered_map<std::string, int> m_symbol_stack_offset_map;
    };
}

#endif
