#include "lexer.h"

#include <iostream>

#define __DEBUG__

Lexer::Lexer(std::string input)
    : m_input(input) {
    readChar();
}

void Lexer::readChar() {
    if (m_read_pos >= static_cast<int>(m_input.length()))
        m_char = 0;
    else
        m_char = m_input[static_cast<size_t>(m_read_pos)];

    m_pos = m_read_pos;
    m_read_pos++;
}

void Lexer::skipWhitespace() {
    while (m_char == ' ' || m_char == '\n' || m_char == '\t' || m_char == '\r')
        readChar();
}

std::string Lexer::readIdentifier() {
    std::string ident;

    while (isLetter(m_char)) {
        ident += m_char;
        readChar();
    }
    
    return ident;
}

std::string Lexer::readNumber() {
    std::string str_num;

    while (isdigit(m_char) || m_char == '.') {
        str_num += m_char;
        readChar();
    }

    return str_num;
}

std::string Lexer::readString() {
    std::string str = "";

    readChar();
    while (true) {
        if (m_char == '"' || m_char == 0)
            break;

        str += m_char;
        readChar();
    }

    return str;
}

char Lexer::peekChar() {
    if (m_read_pos >= static_cast<int>(m_input.length()))
        return 0;
    else
        return m_input[static_cast<size_t>(m_read_pos)];
}

bool Lexer::isLetter(char c) {
    return isalpha(c) || c == '_';
}

Token Lexer::nextToken() {
    Token tok;

    skipWhitespace();

    std::string literal = std::string(1, m_char);

    switch (m_char) {
        case '=':
            if (peekChar() == '=') {
                readChar();
                literal += m_char;
                tok = {TOK_EQ, literal};
            } else {
                tok = {TOK_ASSIGN, literal};
            }
            break;
        case '+':
            tok = {TOK_PLUS, literal}; break;
        case '-':
            tok = {TOK_MINUS, literal}; break;
        case '!':
            if (peekChar() == '=') {
                readChar();
                literal += m_char;
                tok = {TOK_NOT_EQ, literal};
            } else {
                tok = {TOK_BANG, literal};
            }
            break;
        case '*':
            tok = {TOK_MUL, literal}; break;
        case '/':
            tok = {TOK_DIV, literal}; break;
        case '<':
            tok = {TOK_LT, literal}; break;
        case '>':
            tok = {TOK_GT, literal}; break;
        case ';':
            tok = {TOK_SEMICOLON, literal}; break;
        case ',':
            tok = {TOK_COMMA, literal}; break;
        case '(':
            tok = {TOK_LPAREN, literal}; break;
        case ')':
            tok = {TOK_RPAREN, literal}; break;
        case '{':
            tok = {TOK_LBRACE, literal}; break;
        case '}':
            tok = {TOK_RBRACE, literal}; break;
        case '"': {
            std::string str = readString();
            tok = {TOK_STR, str};
            break;
        }
        case 0:
            tok = {TOK_EOF, ""}; break;
        default:
            if (isLetter(m_char)) {
                tok.literal = readIdentifier();
                tok.type = lookupIdent(tok.literal);
                return tok;
            } else if (isdigit(m_char)) {
                const std::string num_str = readNumber();
                return getNumberToken(num_str);
            } else {
#ifdef __DEBUG__
                std::cout << "Illegal tok: " << literal << " found\n";
#endif
                tok = {TOK_ILLEGAL, literal};
            }
            break;
    }
    readChar();

    return tok;
}

Token Lexer::getNumberToken(const std::string& str) {
    const size_t str_len = str.length();
    unsigned int n_dots = 0;

    for (unsigned int i = 0; i < str_len; i++) {
        if (str[i] == '.') {
            if (i == 0)
                return {TOK_ILLEGAL, str};
            else if (i == str_len - 1)
                return {TOK_ILLEGAL, str};

            n_dots++;

            if (n_dots > 1)
                return {TOK_ILLEGAL, str};
        }
    }

    if (n_dots == 1)
        return {TOK_FLOAT, str};

    return {TOK_INT, str};
}
