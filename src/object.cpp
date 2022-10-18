#include "object.h"

#include <vector>

const std::string Bool::inspect() const {
    if (value) return "true";
    return "false";
};

const std::string Function::inspect() const {
    std::string func_str = "func(";
    unsigned int n = params.size();
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
