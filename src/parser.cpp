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

    nextToken();

    statement->setValue(parseExpr(LOWEST));

    while (peekTokenIs(TOK_SEMICOLON))
        nextToken();

    return statement;
}

std::unique_ptr<ReturnStatement> Parser::parseReturnStatement() {
    auto statement = std::make_unique<ReturnStatement>(m_cur_tok);

    nextToken();

    statement->setValue(parseExpr(LOWEST));

    while (peekTokenIs(TOK_SEMICOLON))
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
    std::unique_ptr<Expr> expr = nullptr;

    switch (m_cur_tok.type)
    {
    case TOK_IDENT:
        expr = parseIdentifier(); break;
    case TOK_INT:
        expr = parseIntegerLiteral(); break;
    case TOK_TRUE:
        expr = parseBoolean(); break;
    case TOK_FALSE:
        expr = parseBoolean(); break;
    case TOK_MINUS:
        expr = parsePrefixExpr(); break;
    case TOK_BANG:
        expr = parsePrefixExpr(); break;
    case TOK_LPAREN:
        expr = parseGroupedExpr(); break;
    case TOK_IF:
        expr = parseIfExpr(); break;
    case TOK_FUNC:
        expr = parseFuncLiteral(); break;
    default:
        break;
    }

    if (!expr) {
        const std::string error = "no prefix parse function for " + std::to_string(m_cur_tok.type) + " found";
        m_errors.push_back(error);

        return nullptr;
    }

    auto left_expr = std::move(expr);

    while (!peekTokenIs(TOK_SEMICOLON) && precedence < peekPrecedence()) {
        if (!isInfix(m_peek_tok.type))
            return left_expr;

        nextToken();

        if (m_cur_tok.type == TOK_LPAREN)
            left_expr = parseCallExpr(std::move(left_expr));
        else
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

std::unique_ptr<Expr> Parser::parseBoolean() {
    return std::make_unique<BoolExpr>(m_cur_tok, curTokenIs(TOK_TRUE));
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

    const int precedence = curPrecedence();

    nextToken();

    auto right = parseExpr(precedence);
    infix_expr->setRight(std::move(right));
    
    return infix_expr;
}

std::unique_ptr<Expr> Parser::parseGroupedExpr() {
    nextToken();

    auto expr = parseExpr(LOWEST);

    if (!expectPeek(TOK_RPAREN))
        return nullptr;

    return expr;
}

std::unique_ptr<Expr> Parser::parseIfExpr() {
    auto expr = std::make_unique<IfExpr>(m_cur_tok);

    if (!expectPeek(TOK_LPAREN))
        return nullptr;

    nextToken();
    expr->setCondition(parseExpr(LOWEST));

    if (!expectPeek(TOK_RPAREN))
        return nullptr;
    
    if (!expectPeek(TOK_LBRACE))
        return nullptr;

    expr->setConsequence(parseBlockStatement());

    if (peekTokenIs(TOK_ELSE)) {
        nextToken();

        if (!expectPeek(TOK_LBRACE))
            return nullptr;

        expr->setAlternative(parseBlockStatement());
    }

    return expr;
}

std::unique_ptr<Expr> Parser::parseFuncLiteral() {
    auto func_literal = std::make_unique<FuncLiteral>(m_cur_tok);

    if (!expectPeek(TOK_LPAREN))
        return nullptr;
    
    func_literal->setParams(parseFuncParameters());

    if (!expectPeek(TOK_LBRACE))
        return nullptr;
    
    func_literal->setBody(parseBlockStatement());

    return func_literal;
}

std::unique_ptr<Expr> Parser::parseCallExpr(std::unique_ptr<Expr> func) {
    auto call_expr = std::make_unique<CallExpr>(m_cur_tok, std::move(func));
    call_expr->setArgs(parseCallArgs());

    return call_expr;
}

std::unique_ptr<BlockStatement> Parser::parseBlockStatement() {
    auto block = std::make_unique<BlockStatement>(m_cur_tok);

    nextToken();

    while (!curTokenIs(TOK_RBRACE) && !curTokenIs(TOK_EOF)) {
        auto statement = parseStatement();

        if (statement)
            block->pushStatement(std::move(statement));
        
        nextToken();
    }

    return block;
}

std::vector<Identifier> Parser::parseFuncParameters() {
    std::vector<Identifier> params;

    if (peekTokenIs(TOK_RPAREN)) {
        nextToken();
        return params;
    }

    nextToken();

    params.push_back(Identifier(m_cur_tok, m_cur_tok.literal));

    while (peekTokenIs(TOK_COMMA)) {
        nextToken();
        nextToken();

        params.push_back(Identifier(m_cur_tok, m_cur_tok.literal));
    }

    if (!expectPeek(TOK_RPAREN))
        return {};
    
    return params;
}

std::vector<std::unique_ptr<Expr>> Parser::parseCallArgs() {
    std::vector<std::unique_ptr<Expr>> args;

    if (peekTokenIs(TOK_RPAREN)) {
        nextToken();
        return args;
    }

    nextToken();
    args.push_back(parseExpr(LOWEST));

    while (peekTokenIs(TOK_COMMA)) {
        nextToken();
        nextToken();

        args.push_back(parseExpr(LOWEST));
    }

    if (!expectPeek(TOK_RPAREN))
        return {};
    
    return args;
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
        tok_type == TOK_GT ||
        tok_type == TOK_LPAREN
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
    case TOK_LPAREN:
        return CALL;
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
