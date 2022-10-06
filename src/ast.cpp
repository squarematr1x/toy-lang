#include "ast.h"

const std::string Program::tokenLiteral() const {
    if (static_cast<int>(m_statements.size()) > 0)
        return m_statements[0]->tokenLiteral();

    return "";
}

void Program::pushStatement(std::unique_ptr<Statement> statement) {
    m_statements.push_back(std::move(statement));
}

std::unique_ptr<Statement> Program::getStatement(unsigned int index) {
    if (m_statements.size() == 0)
        return nullptr;

    if (index >= static_cast<unsigned int>(m_statements.size()))
        return nullptr;

    return std::move(m_statements[index]);
}

Identifier::Identifier(Token tok, const std::string& value) 
    : m_tok(tok), m_value(value) {
}

// Identifier::Identifier(const Identifier& ident)
//     : m_tok(ident.getTok()), m_value(ident.getValue()) {
// }


LetStatement::LetStatement(Token tok)
    : m_tok(tok) {
}

ReturnStatement::ReturnStatement(Token tok)
    :m_tok(tok) {
}
