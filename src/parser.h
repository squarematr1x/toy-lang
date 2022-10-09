#pragma once

#include <map>

#include "lexer.h"
#include "ast.h"

enum precedence {
    LOWEST,
    EQUALS,
    LESSGREATER,
    SUM,
    PROD,
    PREFIX,
    CALL
};

class Parser {
    Lexer m_lexer;

    Token m_cur_tok;
    Token m_peek_tok;

    std::vector<std::string> m_errors;

    // TODO: Maps may not be necessary, just use switch?
    // std::map<token_type, int> m_prefix_parse_funcs;
    // std::map<token_type, int> m_infix_parse_funcs; 

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
    std::unique_ptr<Expr> prefixParseFunc();
    std::unique_ptr<Expr> infixParseFunc(std::unique_ptr<Expr> expr);
    

    bool curTokenIs(token_type tok_type);
    bool peekTokenIs(token_type tok_type);
    bool expectPeek(token_type tok_type);
    bool isNumber(Token tok);
};
