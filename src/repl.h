#pragma once

#include <vector>
#include <string>

namespace repl {

void start();
void printParsingErrors(std::vector<std::string> errors);

} // repl