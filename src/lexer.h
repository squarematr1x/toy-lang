#pragma once

#include "token.h"

class Lexer {
    std::string m_input;
    int m_pos {0}; // current position in input (points to current char)
    int m_read_pos {0}; // current reading position in input (after current char)
    char m_char {0}; // current char under examination

public:
    Lexer(std::string input);

    void readChar();
    void skipWhitespace();

    std::string readIdentifier();
    std::string readNumber();
    std::string readString();

    char peekChar();

    bool isLetter(char c);

    Token nextToken();
    Token getNumberToken(const std::string& str);
};
