/*
 * Copyright (c) 2020-2021, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibJS/Runtime/Object.h>

namespace JS {

struct Variable {
    Value value;
    DeclarationKind declaration_kind;
};

#define JS_ENVIRONMENT(class_, base_class) \
public:                                    \
    using Base = base_class;               \
    virtual char const* class_name() const override { return #class_; }

class Environment : public Cell {
public:
    GlobalObject& global_object() { return *m_global_object; }
    GlobalObject const& global_object() const { return *m_global_object; }

    virtual void initialize(GlobalObject&) override;

    virtual bool has_this_binding() const { return false; }
    virtual Value get_this_binding(GlobalObject&) const { return {}; }

    virtual Object* with_base_object() const { return nullptr; }

    virtual bool has_binding([[maybe_unused]] FlyString const& name) const { return false; }
    virtual void create_mutable_binding(GlobalObject&, [[maybe_unused]] FlyString const& name, [[maybe_unused]] bool can_be_deleted) { }
    virtual void create_immutable_binding(GlobalObject&, [[maybe_unused]] FlyString const& name, [[maybe_unused]] bool strict) { }
    virtual void initialize_binding(GlobalObject&, [[maybe_unused]] FlyString const& name, Value) { }
    virtual void set_mutable_binding(GlobalObject&, [[maybe_unused]] FlyString const& name, Value, [[maybe_unused]] bool strict) { }
    virtual Value get_binding_value(GlobalObject&, [[maybe_unused]] FlyString const& name, [[maybe_unused]] bool strict) { return {}; }
    virtual bool delete_binding(GlobalObject&, [[maybe_unused]] FlyString const& name) { return false; }

    // [[OuterEnv]]
    Environment* outer_environment() { return m_outer_environment; }
    Environment const* outer_environment() const { return m_outer_environment; }

    virtual bool is_global_environment() const { return false; }
    virtual bool is_declarative_environment() const { return false; }
    virtual bool is_function_environment() const { return false; }

    template<typename T>
    bool fast_is() const = delete;

    virtual char const* class_name() const override { return "Environment"; }

protected:
    explicit Environment(Environment* parent);

    virtual void visit_edges(Visitor&) override;

private:
    virtual bool is_environment() const final { return true; }

    GlobalObject* m_global_object { nullptr };
    Environment* m_outer_environment { nullptr };
};

}
