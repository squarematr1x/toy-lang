#include "object.h"

#include <vector>

const std::string Bool::inspect() const {
    if (value) return "true";
    return "false";
};

const std::string Array::inspect() const {
    std::string arr_str = "[";

    const size_t n = elements.size();
    for (unsigned int i = 0; i < n; i++) {
        arr_str += elements[i]->inspect();

        if (i < n - 1)
            arr_str += ",";
    }
    arr_str += "]";

    return arr_str;
};

const std::string Function::inspect() const {
    std::string func_str = "func(";
    const size_t n = params.size();
    for (unsigned int i = 0; i < n; i++) {
        func_str += params[i].toString();

        if (i < n - 1)
            func_str += ",";
    }
    
    func_str += ") {\n";
    func_str += body->toString();
    func_str += "\n}";
    
    return func_str;
}
