#pragma once

#include <map>

#include "lexer.h"
#include "ast.h"

enum precedence {
    LOWEST,
    EQUALS,
    LESSGREATER,
    SUM,
    MUL,
    PREFIX,
    CALL
};

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
    std::unique_ptr<ExprStatement> parseExprStatement();
    std::unique_ptr<Expr> parseExpr(int precedence);
    std::unique_ptr<Expr> parseIdentifier();
    std::unique_ptr<Expr> parseIntegerLiteral();
    std::unique_ptr<Expr> parsePrefixExpr();
    std::unique_ptr<Expr> parseInfixExpr(std::unique_ptr<Expr> left);
    
    bool curTokenIs(token_type tok_type);
    bool peekTokenIs(token_type tok_type);
    bool expectPeek(token_type tok_type);
    bool isNumber(Token tok);
    bool isInfix(int tok_type);

    int getPrecedence(int tok_type);
    int peekPrecedence();
    int curPrecedence();
};
