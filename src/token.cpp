#include "token.h"

Token::Token(int type, std::string literal)
    : type(type), literal(literal) {
}

int lookupIdent(std::string ident) {
    std::map<std::string, int> keywords = {
        {"func", TOK_FUNC},
        {"let", TOK_LET},
        {"true", TOK_TRUE},
        {"false", TOK_FALSE},
        {"if", TOK_IF},
        {"else", TOK_ELSE},
        {"return", TOK_RETURN}
    };

    if (keywords[ident])
        return keywords[ident];

    return TOK_IDENT;
}

bool operator== (const Token& tok_a, const Token& tok_b) {
    return (tok_a.type == tok_b.type && tok_a.literal == tok_b.literal);
}

bool operator!= (const Token& tok_a, const Token& tok_b) {
    return !(tok_a == tok_b);
}
