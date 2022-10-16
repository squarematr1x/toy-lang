#pragma once

#include <map>

#include "object.h"

class Env {
    std::map<std::string, std::unique_ptr<Object>> m_store;

public:
    std::unique_ptr<Object> get(const std::string& name);
    std::unique_ptr<Object> set(const std::string& name, std::unique_ptr<Object> value);
};
