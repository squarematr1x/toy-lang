#include "repl.h"
#include "parser.h"
#include "evaluator.h"

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

        const std::string program_str = program->toString();
        auto evaluated = evaluator::evaluate(std::move(program));
    
        if (evaluated)
            std::cout << evaluated->inspect() << '\n';

        std::cout << program_str << '\n';
    }
}

void printParsingErrors(std::vector<std::string> errors) {
    for (const auto& error : errors)
        std::cout << '\t' << error << '\n';
}

} // repl
