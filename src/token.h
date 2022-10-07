#pragma once

#include <string>
#include <map>

enum token_type {
    TOK_EOF = -1,
    TOK_ILLEGAL,

    // Identifiers + literals
    TOK_IDENT, // foo, bar, x, ...
    TOK_INT,

    // Operators
    TOK_ASSIGN,
    TOK_PLUS,
    TOK_MINUS,
    TOK_BANG,
    TOK_MUL,
    TOK_DIV,

    TOK_LT,
    TOK_GT,

    // Delimiters
    TOK_COMMA,
    TOK_SEMICOLON,

    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACE,
    TOK_RBRACE,

    // Keywords
    TOK_FUNC,
    TOK_LET,
    TOK_TRUE,
    TOK_FALSE,
    TOK_IF,
    TOK_ELSE,
    TOK_RETURN,
    TOK_EQ,
    TOK_NOT_EQ
};

struct Token {
    int type;
    std::string literal;

    friend bool operator== (const Token& tok_a, const Token& tok_b);
    friend bool operator!= (const Token& tok_a, const Token& tok_b);

    Token() = default;
    Token(int type, std::string literal);
};

int lookupIdent(std::string ident);
