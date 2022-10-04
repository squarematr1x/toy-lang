#include "repl.h"
#include "lexer.h"

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

        while (true) {
            Token tok = lexer.nextToken();

            if (tok.type == TOK_EOF) 
                break;

            std::cout << tok.type << " : " << tok.literal << '\n';
        }
    }
}

} // repl
