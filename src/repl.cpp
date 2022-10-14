#include "repl.h"
#include "parser.h"
#include "evaluator.h"

#include <iostream>
#include <string>

#define __DEBUG__

namespace repl {

void start() {
    while (true) {
        std::string line;
        std::cout << ">> ";
        std::getline(std::cin, line);

        if (line == "exit")
            break;
        
        Lexer lexer(line);

        // while (true) {
        //     Token tok = lexer.nextToken();

        //     if (tok.type == TOK_EOF) 
        //         break;

        //     std::cout << tok.type << " : " << tok.literal << '\n';
        // }

        Parser parser(lexer);
        auto program = parser.parseProgram();

        if (parser.errors().size() != 0) {
            printParsingErrors(parser.errors());
            break;
        }

        const std::string program_str = program->toString();
        auto evaluated = evaluator::eval(std::move(program));
    
        if (evaluated)
            std::cout << evaluated->inspect() << '\n';

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
