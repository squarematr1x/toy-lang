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
    CALL,
    INDEX,
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

    ExprPtr parseExpr(int precedence);
    ExprPtr parseIdentifier();
    ExprPtr parseIntegerLiteral();
    ExprPtr parseFloatLiteral();
    ExprPtr parseStringLiteral();
    ExprPtr parseBoolean();
    ExprPtr parsePrefixExpr();
    ExprPtr parseInfixExpr(ExprPtr left);
    ExprPtr parseGroupedExpr();
    ExprPtr parseIfExpr();
    ExprPtr parseFuncLiteral();
    ExprPtr parseCallExpr(ExprPtr func);
    ExprPtr parseArrayLiteral();
    ExprPtr parseIndexExpr(ExprPtr left);
    ExprPtr parseHashLiteral();

    std::shared_ptr<BlockStatement> parseBlockStatement();

    std::vector<Identifier> parseFuncParameters();
    std::vector<ExprPtr> parseExprList(token_type end_tok);

    bool curTokenIs(token_type tok_type);
    bool peekTokenIs(token_type tok_type);
    bool expectPeek(token_type tok_type);
    bool isNumber(const Token& tok);
    bool isInfix(int tok_type);

    int getPrecedence(int tok_type);
    int peekPrecedence();
    int curPrecedence();
};
