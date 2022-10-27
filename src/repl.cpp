#include "repl.h"
#include "parser.h"
#include "evaluator.h"
#include "util.h"

#include <iostream>
#include <string>

// #define __DEBUG__

namespace repl {

void start() {
    EnvPtr env = std::make_shared<Env>();

    while (true) {
        std::string line;
        std::cout << ">> ";
        std::getline(std::cin, line);

        if (line == "exit")
            break;
        else if (line == "clear")
            util::clear();
        else {
            Lexer lexer(line);
            Parser parser(lexer);
            auto program = parser.parseProgram();

            if (parser.errors().size() != 0) {
                printParsingErrors(parser.errors());
                break;
            }

            const std::string program_str = program->toString();
            auto evaluated = evaluator::eval((program), env);
        
            if (evaluated)
                std::cout << evaluated->inspect() << '\n';
        }

#ifdef __DEBUG__
        std::cout << "Tree structure: ";
        std::cout << program_str << "\n";
#endif
    }
}

void printParsingErrors(std::vector<std::string> errors) {
    for (const auto& error : errors)
        std::cout << '\t' << error << '\n';
}

} // repl
