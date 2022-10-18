#include "env.h"

#include <iostream>

Env::Env(EnvPtr outer_env) 
    : m_outer_env(outer_env) {
}

ObjectPtr Env::get(const std::string& name) {
    auto search = m_store.find(name);

    if (search != m_store.end()) {
        return search->second;
        // switch (search->second->getType())
        // {
        // case OBJ_INT:
        //     return std::make_unique<Integer>(search->second->getIntVal());
        // case OBJ_BOOL:
        //     return std::make_unique<Bool>(search->second->getBoolVal());
        // case OBJ_FUNC:
        //     return std::make_unique<Function>(search->second->getParams(), search->second->getBody(), search->second->getEnv());
        // default:
        //     return nullptr;
        // }
    } else if (m_outer_env != nullptr)
        return m_outer_env->get(name);

    return nullptr;
}

ObjectPtr Env::set(const std::string& name, ObjectPtr value) {
    m_store[name] = value;

    // switch (value->getType())
    // {
    // case OBJ_INT:
    //     m_store[name] = std::make_unique<Integer>(value->getIntVal()); break;
    // case OBJ_BOOL:
    //     m_store[name] = std::make_unique<Bool>(value->getBoolVal()); break;
    // case OBJ_FUNC:
    //     m_store[name] = std::make_unique<Function>(value->getParams(), value->getBody(), value->getEnv()); break;
    // default:
    //     break;
    // }

    return value;
}

