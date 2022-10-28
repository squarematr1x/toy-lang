#include "builtin.h"

ObjectPtr getBuiltin(const std::string& func_name, const std::vector<ObjectPtr>& args) {
    if (func_name == "len")
        return len(args);
    else if (func_name == "first")
        return first(args);
    else if (func_name == "last")
        return last(args);
    else if (func_name == "push")
        return push(args);
    else if (func_name == "type")
        return type(args);
    else if (func_name == "print")
        return print(args);

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
    case OBJ_ARRAY:
        return std::make_shared<Integer>(static_cast<int>(args[0]->getElements().size()));
    default:
        return std::make_shared<Error>("argument 'len' not supported, got=" + args[0]->typeString());
    }
}

ObjectPtr first(const std::vector<ObjectPtr>& args) {
    const size_t n_args = args.size();

    if (n_args != 1)
        return std::make_shared<Error>("wrong number of arguments. got=" + std::to_string(n_args) + ", want=1");
    if (args[0]->getType() != OBJ_ARRAY)
        return std::make_shared<Error>(("argument to 'first' must be ARRAY, got=" + args[0]->typeString()));

    auto arr = args[0];
    if (arr->getElements().size() > 0)
        return arr->getElements()[0];

    return std::make_shared<NIL>();
}

ObjectPtr last(const std::vector<ObjectPtr>& args) {
    const size_t n_args = args.size();

    if (n_args != 1)
        return std::make_shared<Error>("wrong number of arguments. got=" + std::to_string(n_args) + ", want=1");
    if (args[0]->getType() != OBJ_ARRAY)
        return std::make_shared<Error>(("argument to 'last' must be ARRAY, got=" + args[0]->typeString()));

    auto arr = args[0];
    const size_t len = arr->getElements().size();
    if (len > 0)
        return arr->getElements()[len-1];

    return std::make_shared<NIL>();
}

// Arrays are immutable (currently at least...)
ObjectPtr push(const std::vector<ObjectPtr>& args) {
    const size_t n_args = args.size();

    if (n_args != 2)
        return std::make_shared<Error>("wrong number of arguments. got=" + std::to_string(n_args) + ", want=2");
    if (args[0]->getType() != OBJ_ARRAY)
        return std::make_shared<Error>(("argument to 'push' must be ARRAY, got=" + args[0]->typeString()));

    auto arr = args[0];
    std::vector<ObjectPtr> new_arr;

    for (const auto& element : arr->getElements())
        new_arr.push_back(element->clone());

    new_arr.push_back(args[1]);

    return std::make_shared<Array>(new_arr);
}

ObjectPtr type(const std::vector<ObjectPtr>& args) {
    const size_t n_args = args.size();

    if (n_args != 1)
        return std::make_shared<Error>("wrong number of arguments. got=" + std::to_string(n_args) + ", want=1");
    
    return std::make_shared<String>(("'" + args[0]->typeString() + "'"));
}

ObjectPtr print(const std::vector<ObjectPtr>& args) {
    std::string str_out = "";

    for (const auto& arg : args) {
        if (!isPrintable(arg->getType()))
            return std::make_shared<Error>("can't print object of type=" + arg->typeString());
    
        if (arg->getType() == OBJ_STR)
            str_out += arg->getStrVal();
        else
            str_out += arg->inspect();
    }

    return std::make_shared<String>(str_out);
}

bool isBuiltIn(const std::string& func_name) {
    if (func_name == "len")
        return true;
    if (func_name == "first")
        return true;
    if (func_name == "last")
        return true;
    if (func_name == "push")
        return true;
    if (func_name == "type")
        return true;
    if (func_name == "print")
        return true;

    return false;
}

bool isPrintable(int obj_type) {
    if (obj_type == OBJ_BUILTIN)
        return false;
    if (obj_type == OBJ_FUNC)
        return false;
    if (obj_type == OBJ_RETURN)
        return false;
    if (obj_type == OBJ_ERROR)
        return false;
    
    return true;
}
