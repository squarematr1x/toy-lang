#pragma once

#include <map>
#include <memory>
#include <string>

#include "object.h"

typedef std::shared_ptr<Env> EnvPtr;

class Env {
    std::map<std::string, ObjectPtr> m_store;
    EnvPtr m_outer_env;

public:
    Env() = default;
    Env(EnvPtr outer_env);

    ObjectPtr get(const std::string& name);
    ObjectPtr set(const std::string& name, ObjectPtr value);
};
