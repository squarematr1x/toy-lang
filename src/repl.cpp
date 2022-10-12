#include "repl.h"
#include "lexer.h"
#include "parser.h"

#include <iostream>
#include <string>

namespace repl {

void start() {
    while (true) {
        std::string line;
        std::cout << ">> ";
        std::getline(std::cin, line);

        if (line == "exit")
            break;
        
        Lexer lexer(line);
        Parser parser(lexer);
        auto program = parser.parseProgram();

        if (parser.errors().size() != 0) {
            printParsingErrors(parser.errors());
            break;
        }

        std::cout << program->toString() << '\n';
    }
}

void printParsingErrors(std::vector<std::string> errors) {
    for (const auto& error : errors)
        std::cout << '\t' << error << '\n';
}

} // repl
