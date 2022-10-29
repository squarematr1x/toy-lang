#pragma once

#include "token.h"

class Lexer {
    std::string m_input;
    int m_read_pos {0};
    char m_char {0}; // current char under examination

public:
    Lexer(std::string input);

    void readChar();
    void skipWhitespace();
    void skipComment();

    std::string readIdentifier();
    std::string readNumber();
    std::string readString();

    char peekChar();

    bool isLetter(char c);

    Token nextToken();
    Token getNumberToken(const std::string& str);
};
