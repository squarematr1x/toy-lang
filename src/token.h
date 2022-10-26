#pragma once

#include <string>
#include <map>

// TODO: To get token string for error handling create array and use enum index to get string
// tok_msg = ["TOK_EOF", "TOK_ILLEGAL", "TOK_IDENT", ...]
// tok_msg[m_cur_tok.type] -> "TOK_X" and so on...

enum token_type {
    TOK_EOF = -1,
    TOK_ILLEGAL,

    // Identifiers + literals
    TOK_IDENT, // foo, bar, x, ...
    TOK_INT,
    TOK_FLOAT,
    TOK_STR,

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

    // Parenthesis
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_LBRACKET,
    TOK_RBRACKET,

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
