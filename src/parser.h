#pragma once

#include "lexer.h"
#include "ast.h"

class Parser {
    Lexer m_lexer;
    Token m_cur_tok;
    Token m_peek_tok;
    std::vector<std::string> m_errors;

public:
    Parser(const Lexer& lexer);

    void nextToken();
    void peekError(token_type tok_type);

    const std::vector<std::string> errors() const { return m_errors; }

    std::unique_ptr<Program> parseProgram();
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<LetStatement> parseLetStatement();
    std::unique_ptr<ReturnStatement> parseReturnStatement();

    bool curTokenIs(token_type tok_type);
    bool peekTokenIs(token_type tok_type);
    bool expectPeek(token_type tok_type);
};
