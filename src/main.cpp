#include <iostream>

#include "repl.h"

int main() {
    std::cout << "==== Welcome to toy-lang ====\n\n";

    repl::start();

    return 0;
}
