#include "ast.h"

// TODO: make it work also with pointer Nodes
std::ostream& operator<< (std::ostream& out, Node& node) {
    out << node.toString();
    return out;
}

const std::string Program::tokenLiteral() const {
    if (static_cast<int>(m_statements.size()) > 0)
        return m_statements[0]->tokenLiteral();

    return "";
}

void Program::pushStatement(std::unique_ptr<Statement> statement) {
    m_statements.push_back(std::move(statement));
}

std::string Program::toString() const  {
    std::string statement_str = "";
    for (const auto& statement : m_statements)
        statement_str += statement->toString();

    return statement_str;
}

std::unique_ptr<Statement> Program::getStatementAt(unsigned int index) {
    if (m_statements.size() == 0)
        return nullptr;

    if (index >= static_cast<unsigned int>(m_statements.size()))
        return nullptr;

    return std::move(m_statements[index]);
}

Identifier::Identifier(const Token& tok, const std::string& value) 
    : m_tok(tok), m_value(value) {
}

std::string Identifier::toString() const {
    return m_value;
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

InfixExpr::InfixExpr(const Token& tok, std::unique_ptr<Expr> left, const std::string& oprtr)
    : m_tok(tok), m_left(std::move(left)), m_oprtr(oprtr) {
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
        if_str += "else ";
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

CallExpr::CallExpr(const Token& tok, std::unique_ptr<Expr> func)
    : m_tok(tok), m_func(std::move(func)) {
}

std::string CallExpr::toString() const {
    std::string call_str;

    call_str += m_func->toString();
    call_str += "(";

    const unsigned int n = m_args.size();
    for (unsigned int i = 0; i < n; i++) {
        call_str += m_args[i]->toString();

        if (i < n - 1)
            call_str += ", ";
    }
    
    call_str += ")";

    return call_str;
}

std::unique_ptr<Expr> CallExpr::getArgAt(unsigned int index) {
    if (m_args.size() == 0)
        return nullptr;

    if (index >= static_cast<unsigned int>(m_args.size()))
        return nullptr;

    return std::move(m_args[index]);
}

IntegerLiteral::IntegerLiteral(const Token& tok)
    : m_tok(tok) {
}

Boolean::Boolean(const Token& tok, bool value)
    : m_tok(tok), m_value(value) {
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

    if (m_expr)
        return_stmnt_str += m_expr->toString();

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

void BlockStatement::pushStatement(std::unique_ptr<Statement> statement) {
    m_statements.push_back(std::move(statement));
}

std::string BlockStatement::toString() const {
    std::string out;
    for (const auto& statement: m_statements)
        out += statement->toString();
    
    return out;
}

std::unique_ptr<Statement> BlockStatement::getStatementAt(unsigned int index) {
    if (m_statements.size() == 0)
        return nullptr;

    if (index >= static_cast<unsigned int>(m_statements.size()))
        return nullptr;

    return std::move(m_statements[index]);
}
