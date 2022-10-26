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

    std::shared_ptr<Program> parseProgram();
    std::shared_ptr<Statement> parseStatement();
    std::shared_ptr<LetStatement> parseLetStatement();
    std::shared_ptr<ReturnStatement> parseReturnStatement();
    std::shared_ptr<ExprStatement> parseExprStatement();
    std::shared_ptr<Expr> parseExpr(int precedence);
    std::shared_ptr<Expr> parseIdentifier();
    std::shared_ptr<Expr> parseIntegerLiteral();
    std::shared_ptr<Expr> parseFloatLiteral();
    std::shared_ptr<Expr> parseStringLiteral();
    std::shared_ptr<Expr> parseBoolean();
    std::shared_ptr<Expr> parsePrefixExpr();
    std::shared_ptr<Expr> parseInfixExpr(std::shared_ptr<Expr> left);
    std::shared_ptr<Expr> parseGroupedExpr();
    std::shared_ptr<Expr> parseIfExpr();
    std::shared_ptr<Expr> parseFuncLiteral();
    std::shared_ptr<Expr> parseCallExpr(std::shared_ptr<Expr> func);
    std::shared_ptr<Expr> parseArrayLiteral();

    std::shared_ptr<BlockStatement> parseBlockStatement();

    std::vector<Identifier> parseFuncParameters();
    std::vector<std::shared_ptr<Expr>> parseExprList(token_type end_tok);

    bool curTokenIs(token_type tok_type);
    bool peekTokenIs(token_type tok_type);
    bool expectPeek(token_type tok_type);
    bool isNumber(const Token& tok);
    bool isInfix(int tok_type);

    int getPrecedence(int tok_type);
    int peekPrecedence();
    int curPrecedence();
};
