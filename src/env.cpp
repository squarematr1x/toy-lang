#include "env.h"

Env::Env(EnvPtr outer_env) 
    : m_outer_env(outer_env) {
}

ObjectPtr Env::get(const std::string& name) {
    auto search = m_store.find(name);

    if (search != m_store.end())
        return search->second;
    if (m_outer_env != nullptr)
        return m_outer_env->get(name);

    return nullptr;
}

ObjectPtr Env::set(const std::string& name, ObjectPtr value) {
    m_store[name] = value;

    return std::make_shared<NIL>();
}
