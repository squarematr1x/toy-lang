#pragma once

#include <string>
#include <vector>
#include <memory>

#include "token.h"

enum node_type {
    NODE_BASIC,
    NODE_PROGRAM,
    NODE_EXPR_STMNT,
    NODE_BLOCK_STMNT,
    NODE_RETURN_STMNT,
    NODE_LET_STMNT,
    NODE_FUNC,
    NODE_CALL_EXPR,
    NODE_IF_EXPR,
    NODE_INT,
    NODE_FLOAT,
    NODE_STR,
    NODE_ARRAY,
    NODE_HASH,
    NODE_INDEX,
    NODE_IDENT,
    NODE_BOOL,
    NODE_PREFIX,
    NODE_INFIX
};

const std::vector<std::string> node_lut = {
    "NODE_BASIC",
    "NODE_PROGRAM",
    "NODE_EXPR_STMNT",
    "NODE_BLOCK_STMNT",
    "NODE_RETURN_STMNT",
    "NODE_LET_STMNT",
    "NODE_FUNC",
    "NODE_CALL_EXPR",
    "NODE_IF_EXPR",
    "NODE_INT",
    "NODE_FLOAT",
    "NODE_STR",
    "NODE_ARRAY",
    "NODE_INDEX",
    "NODE_IDENT",
    "NODE_BOOL",
    "NODE_PREFIX",
    "NODE_INFIX"
};

class ASTNode;
class Expr;
class Identifier;
class Statement;
class BlockStatement;

typedef std::shared_ptr<ASTNode> ASTNodePtr;
typedef std::shared_ptr<Expr> ExprPtr;

class ASTNode {
public:
    virtual ~ASTNode() = default;

    virtual const std::string tokenLiteral() const{ return ""; }
    virtual const std::string getIdentName() const { return ""; }
    virtual std::string toString() const { return ""; }

    virtual ExprPtr getExpr() { return nullptr; }
    virtual ExprPtr getLeft() { return nullptr; }
    virtual ExprPtr getRight() { return nullptr; }
    virtual ExprPtr getIndex() { return nullptr; }
    virtual ExprPtr getCondition() { return nullptr; }
    virtual ExprPtr getFunc() { return nullptr; }
    virtual std::shared_ptr<BlockStatement> getConsequence() { return nullptr; }
    virtual std::shared_ptr<BlockStatement> getAlternative() { return nullptr; }
    virtual std::shared_ptr<BlockStatement> getBody() { return nullptr; }

    virtual std::vector<Identifier> getParams() const { return {}; }
    virtual std::vector<std::shared_ptr<Statement>> getStatements() { return {}; }
    virtual std::vector<ExprPtr> getArgs() { return {}; }
    virtual std::vector<ExprPtr> getElements() { return {}; }

    virtual std::map<ExprPtr, ExprPtr> getPairs() { return {}; }

    virtual int nodeType() const { return NODE_BASIC; }
    virtual int getIntValue() const { return -1; }

    virtual double getFloatValue() const { return -1; }

    virtual bool getBoolValue() const { return false; }

    friend std::ostream& operator<< (std::ostream& out, ASTNode& node);
};

class Expr: public ASTNode {
public:
    virtual ~Expr() = default;

    virtual ExprPtr getArgAt(unsigned int index) { (void)index; return nullptr;}
    
    virtual size_t getArgSize() const { return 0; }
};

class Statement: public ASTNode {
public:
    virtual ~Statement() = default;
};

// Root node of AST
class Program: public ASTNode {
    std::vector<std::shared_ptr<Statement>> m_statements;

public:
    void pushStatement(std::shared_ptr<Statement> statement);

    const std::string tokenLiteral() const override;
    std::string toString() const override;

    unsigned int nStatements() const { return static_cast<unsigned int>(m_statements.size()); } 
    int nodeType() const override { return NODE_PROGRAM; }

    std::shared_ptr<Statement> getStatementAt(unsigned int index);

    std::vector<std::shared_ptr<Statement>> getStatements() override { return (m_statements); }
};

class Identifier: public Expr {
    Token m_tok; // Move this to parent?
    std::string m_value;

public:
    Identifier(const Token& tok, const std::string& value);
    Identifier() = default;

    std::string toString() const override;

    const std::string tokenLiteral() const override { return m_tok.literal; }
    const std::string getIdentName() const override { return m_value; }

    int nodeType() const override { return NODE_IDENT; }
};

class IntegerLiteral: public Expr {
    Token m_tok;
    int m_value;

public:
    IntegerLiteral(const Token& tok);

    void setValue(int value) { m_value = value; }

    std::string toString() const override { return m_tok.literal; }
    const std::string tokenLiteral() const override { return m_tok.literal; }

    int nodeType() const override { return NODE_INT; }
    int getIntValue() const override { return m_value; }
};

class FloatLiteral: public Expr {
    Token m_tok;
    double m_value;

public:
    FloatLiteral(const Token& tok);

    void setValue(double value) { m_value = value; }

    std::string toString() const override { return m_tok.literal; }
    const std::string tokenLiteral() const override { return m_tok.literal; }

    int nodeType() const override { return NODE_FLOAT; }
    double getFloatValue() const override { return m_value; }
};

class StringLiteral: public Expr {
    Token m_tok;
    std::string m_value;

public:
    StringLiteral(const Token& tok, const std::string& value);

    std::string toString() const override { return m_tok.literal; }
    const std::string tokenLiteral() const override { return m_tok.literal; }

     int nodeType() const override { return NODE_STR; }
};

class BoolExpr: public Expr {
    Token m_tok;
    bool m_value;

public:
    BoolExpr(const Token& tok, bool value);

    std::string toString() const override { return m_tok.literal; }
    const std::string tokenLiteral() const override { return m_tok.literal; }

    int nodeType() const override { return NODE_BOOL; }

    bool getBoolValue() const override { return m_value; }
};

class ArrayLiteral: public Expr {
    Token m_tok;
    std::vector<ExprPtr> m_elements;

public:
    ArrayLiteral(const Token& tok);

    void setElements(std::vector<ExprPtr> elements) { m_elements = elements; }

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    std::vector<ExprPtr> getElements() override { return m_elements; }

    int nodeType() const override { return NODE_ARRAY; }
};

class IndexExpr: public Expr {
    Token m_tok;
    // <expr>[<expr>]
    ExprPtr m_left;
    ExprPtr m_index;

public:
    IndexExpr(const Token& tok, ExprPtr left);

    void setIndex(ExprPtr index) { m_index = index; }

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    ExprPtr getLeft() override { return m_left; }
    ExprPtr getIndex() override { return m_index; }

    int nodeType() const override { return NODE_INDEX; }
};

class HashLiteral: public Expr {
    Token m_tok;
    std::map<ExprPtr, ExprPtr> m_pairs;

public:
    HashLiteral(const Token& tok);

    void setPairs(std::map<ExprPtr, ExprPtr> pairs) { m_pairs = pairs; }

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    std::map<ExprPtr, ExprPtr> getPairs() override { return m_pairs; }

    int nodeType() const override { return NODE_HASH; }
};

class PrefixExpr: public Expr {
    Token m_tok;
    std::string m_oprtr;
    ExprPtr m_right;

public:
    PrefixExpr(const Token& tok, const std::string& oprtr);

    void setRight(ExprPtr right) { m_right = right; }

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    ExprPtr getRight() override { return m_right; }

    int nodeType() const override { return NODE_PREFIX; }
};

class InfixExpr: public Expr {
    Token m_tok;
    ExprPtr m_left;
    ExprPtr m_right;
    std::string m_oprtr;

public:
    InfixExpr(const Token& tok, ExprPtr left, const std::string& oprtr);

    void setRight(ExprPtr right) { m_right = right; }

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    ExprPtr getLeft() override { return m_left; }
    ExprPtr getRight() override { return m_right; }

    int nodeType() const override { return NODE_INFIX; }
};

class IfExpr: public Expr {
    Token m_tok;
    ExprPtr m_condition;
    std::shared_ptr<BlockStatement> m_consequence;
    std::shared_ptr<BlockStatement> m_alternative;

public:
    IfExpr(Token tok);

    void setCondition(ExprPtr condition) { m_condition = condition; }
    void setConsequence(std::shared_ptr<BlockStatement> consequence) { m_consequence = consequence; }
    void setAlternative(std::shared_ptr<BlockStatement> alternative) { m_alternative = alternative; }

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    ExprPtr getCondition() override { return m_condition; }
    std::shared_ptr<BlockStatement> getConsequence() override { return m_consequence; }
    std::shared_ptr<BlockStatement> getAlternative() override { return m_alternative; }

    int nodeType() const override { return NODE_IF_EXPR; }
};

class FuncLiteral: public Expr {
    Token m_tok;
    std::vector<Identifier> m_params;
    std::shared_ptr<BlockStatement> m_body;

public:
    FuncLiteral(const Token& tok);

    void setParams(const std::vector<Identifier>& params) { m_params = params; }
    void setBody(std::shared_ptr<BlockStatement> body) { m_body = body; }

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    std::shared_ptr<BlockStatement> getBody() override { return m_body; }

    std::vector<Identifier> getParams() const override { return m_params; }

    int nodeType() const override { return NODE_FUNC; }
};

class CallExpr: public Expr {
    Token m_tok;
    ExprPtr m_func;
    std::vector<ExprPtr> m_args;

public:
    CallExpr(const Token& tok, ExprPtr func);

    void setArgs(std::vector<ExprPtr> args) { m_args = args; }

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    ExprPtr getFunc() override { return m_func; }
    ExprPtr getArgAt(unsigned int index) override;

    size_t getArgSize() const override { return m_args.size(); }

    std::vector<ExprPtr> getArgs() override { return m_args; }

    int nodeType() const override { return NODE_CALL_EXPR; }
};

class LetStatement: public Statement {
    Token m_tok;
    Identifier m_name;
    ExprPtr m_value;

public:
    LetStatement(const Token& tok);

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }
    const std::string getIdentName() const override { return m_name.getIdentName(); }

    void setName(const Identifier ident) { m_name = ident; }
    void setValue(ExprPtr expr) { m_value = expr; }

    ExprPtr getExpr() override { return m_value; }

    int nodeType() const override { return NODE_LET_STMNT; }
};

class ReturnStatement: public Statement {
    Token m_tok;
    ExprPtr m_value;

public:
    ReturnStatement(const Token& tok);

    void setValue(ExprPtr expr) { m_value = expr; }
    
    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    ExprPtr getExpr() override { return m_value; }

    int nodeType() const override { return NODE_RETURN_STMNT; }
};

class ExprStatement: public Statement {
    Token m_tok;
    ExprPtr m_expr;

public:
    ExprStatement(const Token& tok);

    void setExpr(ExprPtr expr) { m_expr = expr; }

    std::string toString() const override;

    const std::string tokenLiteral() const override { return m_tok.literal; }

    ExprPtr getExpr() override { return m_expr; }

    int nodeType() const override { return NODE_EXPR_STMNT; }
};

class BlockStatement: public Statement {
    Token m_tok;
    std::vector<std::shared_ptr<Statement>> m_statements;

public:
    BlockStatement(const Token& tok);

    void pushStatement(std::shared_ptr<Statement> statement);

    std::string toString() const override;
    const std::string tokenLiteral() const override { return m_tok.literal; }

    std::shared_ptr<Statement> getStatementAt(unsigned int index);

    int nodeType() const override { return NODE_BLOCK_STMNT; }

    std::vector<std::shared_ptr<Statement>> getStatements() override { return m_statements; }
};
