#pragma once

#include "ast.h"
#include "object.h"

namespace evaluator {

std::unique_ptr<Object> evaluate(const std::unique_ptr<Node>& node);
std::unique_ptr<Object> evalStatements(std::vector<std::unique_ptr<Statement>> statements);
std::unique_ptr<Object> evalPrefixExpr(const std::string& oprtr, const std::unique_ptr<Object>& right);
std::unique_ptr<Object> evalInfixExpr(const std::string& oprtr, const std::unique_ptr<Object>& left, const std::unique_ptr<Object>& right);
std::unique_ptr<Object> evalBangOperator(const std::unique_ptr<Object>& right);
std::unique_ptr<Object> evalMinusOperator(const std::unique_ptr<Object>& right);
std::unique_ptr<Object> evalIntInfixExpr(const std::string& oprtr, const std::unique_ptr<Object>& left, const std::unique_ptr<Object>& right);
std::unique_ptr<Object> evalIfExpr(const std::unique_ptr<Node>& node);

bool isTrue(const std::unique_ptr<Object>& obj);

} // evaluator
