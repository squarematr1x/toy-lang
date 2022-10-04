#include "token.h"

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
