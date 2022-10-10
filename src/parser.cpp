#include "parser.h"

#include <algorithm>
#include <iostream>

Parser::Parser(const Lexer& lexer)
    : m_lexer(lexer) {
    nextToken();
    nextToken();
}

void Parser::nextToken() {
    m_cur_tok = m_peek_tok;
    m_peek_tok = m_lexer.nextToken();
}

void Parser::peekError(token_type tok_type) {
    std::string msg = "expected next token to be "
        + std::to_string(tok_type) + ", got "
        + std::to_string(m_peek_tok.type)
        + " instead";
    
    m_errors.push_back(msg);
}

std::unique_ptr<Program> Parser::parseProgram() {
    auto program = std::make_unique<Program>();

    while (m_cur_tok.type != TOK_EOF) {
        auto statement = parseStatement();

        if (statement)
            program->pushStatement(std::move(statement));

        nextToken();
    }

    return program;
}

std::unique_ptr<Statement> Parser::parseStatement() {
    switch (m_cur_tok.type)
    {
    case TOK_LET:
        return parseLetStatement();
    case TOK_RETURN:
        return parseReturnStatement();
    default:
        return parseExprStatement();
    }
}

std::unique_ptr<LetStatement> Parser::parseLetStatement() {
    auto statement = std::make_unique<LetStatement>(m_cur_tok);

    if (!expectPeek(TOK_IDENT))
        return nullptr;

    Identifier ident(m_cur_tok, m_cur_tok.literal);
    statement->setName(ident);

    if (!expectPeek(TOK_ASSIGN))
        return nullptr;

    // Skipping the expressions until we encounter a semicolon
    while (!curTokenIs(TOK_SEMICOLON))
        nextToken();

    return statement;
}

std::unique_ptr<ReturnStatement> Parser::parseReturnStatement() {
    auto statement = std::make_unique<ReturnStatement>(m_cur_tok);

    nextToken();

    // Skipping the expressions until we encounter a semicolon
    while (!curTokenIs(TOK_SEMICOLON))
        nextToken();

    return statement;
}

std::unique_ptr<ExprStatement> Parser::parseExprStatement() {
    auto statement = std::make_unique<ExprStatement>(m_cur_tok);
    auto expr = parseExpr(LOWEST);

    statement->setExpr(std::move(expr));
    if (peekTokenIs(TOK_SEMICOLON))
        nextToken();

    return statement;
}

std::unique_ptr<Expr> Parser::parseExpr(int precedence) {
    std::unique_ptr<Expr> prefix = nullptr;

    switch (m_cur_tok.type)
    {
    case TOK_IDENT:
        prefix = parseIdentifier(); break;
    case TOK_INT:
        prefix = parseIntegerLiteral(); break;
    case TOK_MINUS:
        prefix = parsePrefixExpr(); break;
    case TOK_BANG:
        prefix = parsePrefixExpr(); break;
    default:
        break;
    }

    if (!prefix) {
        const std::string error = "no prefix parse function for " + std::to_string(m_cur_tok.type) + " found";
        m_errors.push_back(error);

        return nullptr;
    }

    auto left_expr = std::move(prefix);

    while (!peekTokenIs(TOK_SEMICOLON) && precedence < peekPrecedence()) {
        std::unique_ptr<Expr> infix = nullptr;

        if (!isInfix(m_peek_tok.type))
            return left_expr;

        nextToken();

        left_expr = parseInfixExpr(std::move(left_expr));
    }

    return left_expr;
}

std::unique_ptr<Expr> Parser::parseIdentifier() {
    auto ident = std::make_unique<Identifier>(m_cur_tok, m_cur_tok.literal);

    return ident;
}

std::unique_ptr<Expr> Parser::parseIntegerLiteral() {
    auto int_lit = std::make_unique<IntegerLiteral>(m_cur_tok);

    if (!isNumber(m_cur_tok)) {
        const std::string error = "could not parse " + m_cur_tok.literal + " as integer";
        m_errors.push_back(error);
        return nullptr;
    }

    int_lit->setValue(stoi(m_cur_tok.literal));

    return int_lit;
}


std::unique_ptr<Expr> Parser::parsePrefixExpr() {
    auto expr = std::make_unique<PrefixExpr>(m_cur_tok, m_cur_tok.literal);

    nextToken();

    auto right_expr = parseExpr(PREFIX);
    expr->setRight(std::move(right_expr));

    return expr;
}

std::unique_ptr<Expr> Parser::parseInfixExpr(std::unique_ptr<Expr> left) {
    auto infix_expr = std::make_unique<InfixExpr>(m_cur_tok, std::move(left), m_cur_tok.literal);

    int precedence = curPrecedence();

    nextToken();

    auto right = parseExpr(precedence);
    infix_expr->setRight(std::move(right));
    
    return infix_expr;
}

bool Parser::curTokenIs(token_type tok_type) {
    return m_cur_tok.type == tok_type;
}

bool Parser::peekTokenIs(token_type tok_type) {
    return m_peek_tok.type == tok_type;
}

bool Parser::expectPeek(token_type tok_type) {
    if (peekTokenIs(tok_type)) {
        nextToken();
        return true;
    }
    peekError(tok_type);

    return false;
}

bool Parser::isNumber(const Token& tok) {
    const std::string str = tok.literal;

    return !str.empty() && std::find_if(str.begin(),
        str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end();
}

bool Parser::isInfix(int tok_type) {
    if (tok_type == TOK_EQ ||
        tok_type == TOK_NOT_EQ ||
        tok_type == TOK_PLUS ||
        tok_type == TOK_MINUS ||
        tok_type == TOK_MUL ||
        tok_type == TOK_DIV ||
        tok_type == TOK_LT ||
        tok_type == TOK_GT
    ) {
        return true;
    }

    return false;
}

int Parser::getPrecedence(int tok_type) {
    switch (tok_type)
    {
    case TOK_EQ:
        return EQUALS;
    case TOK_NOT_EQ:
        return EQUALS;
    case TOK_PLUS:
        return SUM;
    case TOK_MINUS:
        return SUM;
    case TOK_MUL:
        return MUL;
    case TOK_DIV:
        return MUL;
    case TOK_LT:
        return LESSGREATER;
    case TOK_GT:
        return LESSGREATER;
    default:
        return LOWEST;
    }
}

int Parser::peekPrecedence() {
    return getPrecedence(m_peek_tok.type);
}

int Parser::curPrecedence() {
    return getPrecedence(m_cur_tok.type);
}
