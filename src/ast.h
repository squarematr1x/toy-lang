#pragma once

#include <string>
#include <vector>
#include <memory>

#include "token.h"

class Node {
public:
    virtual ~Node() = default;
    virtual const std::string tokenLiteral() const = 0;
    virtual std::string toString() const = 0;

    friend std::ostream& operator<< (std::ostream& out, Node& node);
};

class Expression: public Node {
public:
    virtual ~Expression() = default;
    virtual void expressionNode() const = 0;

    std::string toString() const override { return ""; }
};

class Statement: public Node {
public:
    virtual ~Statement() = default;
    virtual void statementNode() const = 0;

    std::string toString() const override { return ""; }
};

// Root node of AST
class Program: public Node {
    std::vector<std::unique_ptr<Statement>> m_statements;

public:
    const std::string tokenLiteral() const override;
    void pushStatement(std::unique_ptr<Statement> statement);
    std::string toString() const override;

    int nStatements() const { return static_cast<int>(m_statements.size()); } 
    
    std::unique_ptr<Statement> getStatement(unsigned int index);
};

class Identifier: public Expression {
    Token m_tok;
    std::string m_value;

public:
    Identifier(Token tok, const std::string& value);
    Identifier() = default;

    void expressionNode() const override {}
    std::string toString() const override;

    const std::string tokenLiteral() const override { return m_tok.literal; }

    const Token getTok() const { return m_tok; }
    const std::string getValue() const { return m_value; }
};

class LetStatement: public Statement {
    Token m_tok;
    Identifier m_name;
    std::unique_ptr<Expression> m_value;

public:
    LetStatement(Token tok);

    void statementNode() const override {}
    std::string toString() const override;
    void setName(const Identifier ident) { m_name = ident; }
    void setValue(std::unique_ptr<Expression> expr) { m_value = std::move(expr); }

    const std::string tokenLiteral() const override { return m_tok.literal; }
};

class ReturnStatement: public Statement {
    Token m_tok;
    std::unique_ptr<Expression> m_expression;

public:
    ReturnStatement(Token tok);

    void statementNode() const override {}
    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }
};

class ExpressionStatement: public Statement {
    Token m_tok;
    std::unique_ptr<Expression> m_expression;

public:
    void statementNode() const override {}
    std::string toString() const override;

    const std::string tokenLiteral() const override { return m_tok.literal; }
};
