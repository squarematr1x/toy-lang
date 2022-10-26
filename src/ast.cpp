#include "ast.h"

// TODO: make it work also with pointer Nodes
std::ostream& operator<< (std::ostream& out, ASTNode& node) {
    out << node.toString();
    return out;
}

const std::string Program::tokenLiteral() const {
    if (static_cast<int>(m_statements.size()) > 0)
        return m_statements[0]->tokenLiteral();

    return "";
}

void Program::pushStatement(std::shared_ptr<Statement> statement) {
    m_statements.push_back(statement);
}

std::string Program::toString() const  {
    std::string statement_str = "";
    for (const auto& statement : m_statements)
        statement_str += statement->toString();

    return statement_str;
}

std::shared_ptr<Statement> Program::getStatementAt(unsigned int index) {
    if (m_statements.size() == 0)
        return nullptr;

    if (index >= static_cast<unsigned int>(m_statements.size()))
        return nullptr;

    return (m_statements[index]);
}

Identifier::Identifier(const Token& tok, const std::string& value) 
    : m_tok(tok), m_value(value) {
}

std::string Identifier::toString() const {
    return m_value;
}

StringLiteral::StringLiteral(const Token& tok, const std::string& value)
    : m_tok(tok), m_value(value) {
}

PrefixExpr::PrefixExpr(const Token& tok, const std::string& oprtr)
    : m_tok(tok), m_oprtr(oprtr) {
}

std::string PrefixExpr::toString() const {
    std::string prefix_str = "(";
    prefix_str += m_oprtr;
    prefix_str += m_right->toString();
    prefix_str += ")";

    return prefix_str;
}

InfixExpr::InfixExpr(const Token& tok, std::shared_ptr<Expr> left, const std::string& oprtr)
    : m_tok(tok), m_left((left)), m_oprtr(oprtr) {
}

std::string InfixExpr::toString() const {
    std::string infix_str = "(";
    infix_str += m_left->toString();
    infix_str += " " + m_oprtr + " ";
    infix_str += m_right->toString();
    infix_str += ")";

    return infix_str;
}

IfExpr::IfExpr(Token tok)
    : m_tok(tok) {
}

std::string IfExpr::toString() const {
    std::string if_str = "if " + m_condition->toString() + " " + m_consequence->toString();

    if (m_alternative) {
        if_str += " else ";
        if_str += m_alternative->toString();
    }
    
    return if_str;
}

FuncLiteral::FuncLiteral(const Token& tok)
    : m_tok(tok) {
}

std::string FuncLiteral::toString() const {
    std::string func_literal_str = m_tok.literal + "(";

    for (const auto& param : m_params)
        func_literal_str += param.toString();
    
    func_literal_str += ") ";
    func_literal_str += m_body->toString();

    return func_literal_str;
}

CallExpr::CallExpr(const Token& tok, std::shared_ptr<Expr> func)
    : m_tok(tok), m_func(func) {
}

std::string CallExpr::toString() const {
    std::string call_str;

    call_str += m_func->toString();
    call_str += "(";

    const size_t n = m_args.size();
    for (unsigned int i = 0; i < n; i++) {
        call_str += m_args[i]->toString();

        if (i < n - 1)
            call_str += ", ";
    }
    
    call_str += ")";

    return call_str;
}

std::shared_ptr<Expr> CallExpr::getArgAt(unsigned int index) {
    if (m_args.size() == 0)
        return nullptr;

    if (index >= static_cast<unsigned int>(m_args.size()))
        return nullptr;

    return (m_args[index]);
}

IntegerLiteral::IntegerLiteral(const Token& tok)
    : m_tok(tok) {
}

FloatLiteral::FloatLiteral(const Token& tok)
    : m_tok(tok) {
}

BoolExpr::BoolExpr(const Token& tok, bool value)
    : m_tok(tok), m_value(value) {
}

ArrayLiteral::ArrayLiteral(const Token& tok)
    : m_tok(tok) {
}

std::string ArrayLiteral::toString() const {
    std::string arr_str = "[";

    const size_t n = m_elements.size();
    for (unsigned int i = 0; i < n; i++) {
        arr_str += m_elements[i]->toString();

        if (i < n - 1)
            arr_str += ", ";
    }
    arr_str += "]";

    return arr_str;
}

LetStatement::LetStatement(const Token& tok)
    : m_tok(tok) {
}

std::string LetStatement::toString() const {
    std::string let_stmnt_str = "";

    let_stmnt_str += tokenLiteral();
    let_stmnt_str += " ";
    let_stmnt_str += m_name.toString();
    let_stmnt_str += " = ";
    
    if (m_value)
        let_stmnt_str += m_value->toString();

    return let_stmnt_str;
}

ReturnStatement::ReturnStatement(const Token& tok)
    :m_tok(tok) {
}

std::string ReturnStatement::toString() const {
    std::string return_stmnt_str = "";

    return_stmnt_str += tokenLiteral();
    return_stmnt_str += " ";

    if (m_value)
        return_stmnt_str += m_value->toString();

    return_stmnt_str += ";";

    return return_stmnt_str;
}

ExprStatement::ExprStatement(const Token& tok)
    : m_tok(tok) {
}

std::string ExprStatement::toString() const {
    if (m_expr)
        return m_expr->toString();

    return "";
}

BlockStatement::BlockStatement(const Token& tok)
    : m_tok(tok) {
}

void BlockStatement::pushStatement(std::shared_ptr<Statement> statement) {
    m_statements.push_back(statement);
}

std::string BlockStatement::toString() const {
    std::string out;
    for (const auto& statement: m_statements)
        out += statement->toString();
    
    return out;
}

std::shared_ptr<Statement> BlockStatement::getStatementAt(unsigned int index) {
    if (m_statements.size() == 0)
        return nullptr;

    if (index >= static_cast<unsigned int>(m_statements.size()))
        return nullptr;

    return (m_statements[index]);
}
