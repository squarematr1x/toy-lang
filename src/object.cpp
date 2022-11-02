#include "object.h"

#include <vector>

bool operator== (const HashKey& hash_key_a, const HashKey& hash_key_b) {
    return hash_key_a.value == hash_key_b.value;
}

bool operator< (const HashKey& hash_key_a, const HashKey& hash_key_b) {
    return hash_key_a.value < hash_key_b.value;
}

const std::string Bool::inspect() const {
    if (value) return "true";
    return "false";
};

HashKey String::hashKey() const {
    std::hash<std::string> hasher;
    auto hashed_value = hasher(value);

    return {OBJ_STR, static_cast<int>(hashed_value)};
}

HashPairPtr Hash::getPairAt(HashKey key) {
    auto search = pairs.find(key);

    if (search != pairs.end())
        return search->second;

    return nullptr;
}

const std::string Array::inspect() const {
    std::string arr_str = "[";

    const size_t n = elements.size();
    for (unsigned int i = 0; i < n; i++) {
        arr_str += elements[i]->inspect();

        if (i < n - 1)
            arr_str += ", ";
    }
    arr_str += "]";

    return arr_str;
};

const std::string Hash::inspect() const {
    std::string hash_str = "{";
    const size_t pairs_size = pairs.size();

    unsigned int i = 0;
    for (const auto& [key, pair]: pairs) {
        hash_str += pair->key->inspect();
        hash_str += " : ";
        hash_str += pair->value->inspect();

        if (i < pairs_size - 1)
            hash_str += ", "; 
        i++;
    }
    hash_str += "}";

    return hash_str;
}

const std::string Function::inspect() const {
    std::string func_str = "func(";
    const size_t n = params.size();
    for (unsigned int i = 0; i < n; i++) {
        func_str += params[i].toString();

        if (i < n - 1)
            func_str += ", ";
    }
    
    func_str += ") {\n";
    func_str += body->toString();
    func_str += "\n}";
    
    return func_str;
}
