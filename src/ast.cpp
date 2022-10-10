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

Identifier::Identifier(Token tok, const std::string& value) 
    : m_tok(tok), m_value(value) {
}

std::string Identifier::toString() const {
    return m_value;
}

PrefixExpr::PrefixExpr(Token tok, const std::string& oprtr)
    : m_tok(tok), m_oprtr(oprtr) {
}

std::string PrefixExpr::toString() const {
    std::string prefix_str = "(";
    prefix_str += m_oprtr;
    prefix_str += m_right->toString();
    prefix_str += ")";

    return prefix_str;
}

InfixExpr::InfixExpr(Token tok, std::unique_ptr<Expr> left, const std::string& oprtr)
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

IntegerLiteral::IntegerLiteral(Token tok)
    : m_tok(tok) {
}

LetStatement::LetStatement(Token tok)
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

ReturnStatement::ReturnStatement(Token tok)
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

ExprStatement::ExprStatement(Token tok)
    : m_tok(tok) {
}

std::string ExprStatement::toString() const {
    if (m_expr)
        return m_expr->toString();

    return "";
}
