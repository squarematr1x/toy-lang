#pragma once

#include "object.h"

ObjectPtr getBuiltin(const std::string& func_name, const std::vector<ObjectPtr>& args);
ObjectPtr len(const std::vector<ObjectPtr>& args);
ObjectPtr first(const std::vector<ObjectPtr>& args);
ObjectPtr last(const std::vector<ObjectPtr>& args);
ObjectPtr push(const std::vector<ObjectPtr>& args);

bool isBuiltIn(const std::string& func_name);

// TODO: Add math functions etc.
// ObjectPtr sin(const std::vector<ObjectPtr>& args);
// ObjectPtr cos(const std::vector<ObjectPtr>& args);
// ObjectPtr tan(const std::vector<ObjectPtr>& args);
// ObjectPtr pow(const std::vector<ObjectPtr>& args);
