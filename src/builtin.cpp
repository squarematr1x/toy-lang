#include "builtin.h"

ObjectPtr getBuiltin(const std::string& func_name, const std::vector<ObjectPtr>& args) {
    if (func_name == "len")
        return len(args);

    return std::make_shared<Error>("identifier not found: " + func_name);
}

ObjectPtr len(const std::vector<ObjectPtr>& args) {
    const size_t n_args = args.size();
    
    if (n_args != 1)
        return std::make_shared<Error>("wrong number of arguments. got=" + std::to_string(n_args) + ", want=1");
    
    switch (args[0]->getType())
    {
    case OBJ_STR:
        return std::make_shared<Integer>(static_cast<int>(args[0]->getStrVal().length()));
    default:
        return std::make_shared<Error>("argument 'len' not supported, got=" + args[0]->typeString());
    }
}

bool isBuiltIn(const std::string& func_name) {
    if (func_name == "len")
        return true;

    return false;
}
