#include "parser.h"

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

        if (statement) {
            program->pushStatement(std::move(statement));
        }
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
        return nullptr;
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
    while (!curTokenIs(TOK_SEMICOLON)) {
        nextToken();
    }

    return statement;
}

std::unique_ptr<ReturnStatement> Parser::parseReturnStatement() {
    auto statement = std::make_unique<ReturnStatement>(m_cur_tok);

    nextToken();

    // Skipping the expressions until we encounter a semicolon
    while (!curTokenIs(TOK_SEMICOLON)) {
        nextToken();
    }

    return statement;
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
