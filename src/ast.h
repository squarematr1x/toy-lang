#pragma once

#include <string>
#include <vector>
#include <memory>

#include "token.h"

class Identifier;
class BlockStatement;

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

    virtual std::unique_ptr<Expr> getLeft() { return nullptr; }
    virtual std::unique_ptr<Expr> getRight() { return nullptr; }
    virtual std::unique_ptr<Expr> getCondition() { return nullptr; }
    virtual std::unique_ptr<Expr> getFunc() { return nullptr; }
    virtual std::unique_ptr<Expr> getArgAt(unsigned int index) { (void)index; return nullptr;}

    virtual std::unique_ptr<BlockStatement> getConsequence() { return nullptr; }
    virtual std::unique_ptr<BlockStatement> getAlternative() { return nullptr; }
    virtual std::unique_ptr<BlockStatement> getBody() { return nullptr; }

    virtual std::vector<Identifier> getParams() const { return {}; }
    
    virtual size_t getArgSize() const { return 0; }
};

class Statement: public Node {
public:
    virtual ~Statement() = default;
    virtual void statementNode() const = 0;

    virtual std::unique_ptr<Expr> getExpr() { return nullptr; }
};

// Root node of AST
class Program: public Node {
    std::vector<std::unique_ptr<Statement>> m_statements;

public:
    void pushStatement(std::unique_ptr<Statement> statement);

    const std::string tokenLiteral() const override;
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
};

class Boolean: public Expr {
    Token m_tok;
    bool m_value;

public:
    Boolean(const Token& tok, bool value);

    void expressionNode() const override {}

    std::string toString() const override { return m_tok.literal; }
    const std::string tokenLiteral() const override { return m_tok.literal; }
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

class IfExpr: public Expr {
    Token m_tok;
    std::unique_ptr<Expr> m_condition;
    std::unique_ptr<BlockStatement> m_consequence;
    std::unique_ptr<BlockStatement> m_alternative;

public:
    IfExpr(Token tok);

    void expressionNode() const override {}
    void setCondition(std::unique_ptr<Expr> condition) { m_condition = std::move(condition); }
    void setConsequence(std::unique_ptr<BlockStatement> consequence) { m_consequence = std::move(consequence); }
    void setAlternative(std::unique_ptr<BlockStatement> alternative) { m_alternative = std::move(alternative); }

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    std::unique_ptr<Expr> getCondition() override { return std::move(m_condition); }
    std::unique_ptr<BlockStatement> getConsequence() override { return std::move(m_consequence); }
    std::unique_ptr<BlockStatement> getAlternative() override { return std::move(m_alternative); }
};

class FuncLiteral: public Expr {
    Token m_tok;
    std::vector<Identifier> m_params;
    std::unique_ptr<BlockStatement> m_body;

public:
    FuncLiteral(const Token& tok);

    void expressionNode() const override {}
    void setParams(const std::vector<Identifier>& params) { m_params = params; }
    void setBody(std::unique_ptr<BlockStatement> body) { m_body = std::move(body); }

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    std::unique_ptr<BlockStatement> getBody() override { return std::move(m_body); }

    std::vector<Identifier> getParams() const override { return m_params; }
};

class CallExpr: public Expr {
    Token m_tok;
    std::unique_ptr<Expr> m_func;
    std::vector<std::unique_ptr<Expr>> m_args;

public:
    CallExpr(const Token& tok, std::unique_ptr<Expr> func);
    void expressionNode() const override {}
    void setArgs(std::vector<std::unique_ptr<Expr>> args) { m_args = std::move(args); }

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    std::unique_ptr<Expr> getFunc() override { return std::move(m_func); }

    size_t getArgSize() const override { return m_args.size(); }

    std::unique_ptr<Expr> getArgAt(unsigned int index) override;
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
};

class ReturnStatement: public Statement {
    Token m_tok;
    std::unique_ptr<Expr> m_value;

public:
    ReturnStatement(const Token& tok);

    void statementNode() const override {}
    void setValue(std::unique_ptr<Expr> expr) { m_value = std::move(expr); }
    
    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }
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

class BlockStatement: public Statement {
    Token m_tok;
    std::vector<std::unique_ptr<Statement>> m_statements;

public:
    BlockStatement(const Token& tok);

    void statementNode() const override {}
    void pushStatement(std::unique_ptr<Statement> statement);

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    std::unique_ptr<Statement> getStatementAt(unsigned int index);
};
