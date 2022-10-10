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

class Expr: public Node {
public:
    virtual ~Expr() = default;
    virtual void expressionNode() const = 0;

    virtual std::unique_ptr<Expr> getLeft() = 0;
    virtual std::unique_ptr<Expr> getRight() = 0;

    std::string toString() const override { return ""; }
};

class Statement: public Node {
public:
    virtual ~Statement() = default;
    virtual void statementNode() const = 0;

    virtual std::unique_ptr<Expr> getExpr() = 0;

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
    
    std::unique_ptr<Statement> getStatementAt(unsigned int index);
};

class Identifier: public Expr {
    Token m_tok; // Move this to parent?
    std::string m_value;

public:
    Identifier(const Token& tok, const std::string& value);
    Identifier() = default;

    void expressionNode() const override {}
    std::string toString() const override;

    const std::string tokenLiteral() const override { return m_tok.literal; }

    const std::string getValue() const { return m_value; }

    std::unique_ptr<Expr> getLeft() override { return nullptr; }
    std::unique_ptr<Expr> getRight() override { return nullptr; }
};

class IntegerLiteral: public Expr {
    Token m_tok;
    int m_value;

public:
    IntegerLiteral(const Token& tok);

    void expressionNode() const override {}
    void setValue(int value) { m_value = value; }

    std::string toString() const override { return m_tok.literal; }
    const std::string tokenLiteral() const override { return m_tok.literal; }

    std::unique_ptr<Expr> getLeft() override { return nullptr; }
    std::unique_ptr<Expr> getRight() override { return nullptr; }
};

class PrefixExpr: public Expr {
    Token m_tok;
    std::string m_oprtr;
    std::unique_ptr<Expr> m_right;

public:
    PrefixExpr(const Token& tok, const std::string& oprtr);

    void expressionNode() const override {}
    void setRight(std::unique_ptr<Expr> right) { m_right = std::move(right); }

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    std::unique_ptr<Expr> getLeft() override { return nullptr; }
    std::unique_ptr<Expr> getRight() override { return std::move(m_right); }
};

class InfixExpr: public Expr {
    Token m_tok;
    std::unique_ptr<Expr> m_left;
    std::unique_ptr<Expr> m_right;
    std::string m_oprtr;

public:
    InfixExpr(const Token& tok, std::unique_ptr<Expr> left, const std::string& oprtr);

    void expressionNode() const override {}
    void setRight(std::unique_ptr<Expr> right) { m_right = std::move(right); }

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    std::unique_ptr<Expr> getLeft() override { return std::move(m_left); }
    std::unique_ptr<Expr> getRight() override { return std::move(m_right); }
};

class LetStatement: public Statement {
    Token m_tok;
    Identifier m_name;
    std::unique_ptr<Expr> m_value;

public:
    LetStatement(const Token& tok);

    void statementNode() const override {}
    std::string toString() const override;
    void setName(const Identifier ident) { m_name = ident; }
    void setValue(std::unique_ptr<Expr> expr) { m_value = std::move(expr); }

    const std::string tokenLiteral() const override { return m_tok.literal; }

    std::unique_ptr<Expr> getExpr() override { return nullptr; }
};

class ReturnStatement: public Statement {
    Token m_tok;
    std::unique_ptr<Expr> m_expr;

public:
    ReturnStatement(const Token& tok);

    void statementNode() const override {}
    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    std::unique_ptr<Expr> getExpr() override { return nullptr; }
};

class ExprStatement: public Statement {
    Token m_tok;
    std::unique_ptr<Expr> m_expr;

public:
    ExprStatement(const Token& tok);

    void statementNode() const override {}
    void setExpr(std::unique_ptr<Expr> expr) { m_expr = std::move(expr); }

    std::string toString() const override;

    const std::string tokenLiteral() const override { return m_tok.literal; }

    std::unique_ptr<Expr> getExpr() override { return std::move(m_expr); }
};
