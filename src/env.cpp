#include "env.h"

#include <iostream>

std::unique_ptr<Object> Env::get(const std::string& name) {
    auto search = m_store.find(name);

    if (search != m_store.end()) {
        switch (search->second->getType())
        {
        case OBJ_INT:
            return std::make_unique<Integer>(search->second->getIntVal());
        case OBJ_BOOL:
            return std::make_unique<Bool>(search->second->getBoolVal());
        default:
            return nullptr;
        }
    }

    return nullptr;
}

std::unique_ptr<Object> Env::set(const std::string& name, std::unique_ptr<Object> value) {
    switch (value->getType())
    {
    case OBJ_INT:
        m_store[name] = std::make_unique<Integer>(value->getIntVal()); break;
    case OBJ_BOOL:
        m_store[name] = std::make_unique<Bool>(value->getBoolVal()); break;
    default:
        break;
    }

    return value;
}
