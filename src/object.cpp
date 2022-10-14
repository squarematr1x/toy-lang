#include "object.h"

const std::string Bool::inspect() const {
    if (value)
        return "true";
    return "false";
};
