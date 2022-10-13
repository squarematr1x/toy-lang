#pragma once

#include "ast.h"
#include "object.h"

namespace evaluator {

std::unique_ptr<Object> evaluate(const std::unique_ptr<Node>& node);
std::unique_ptr<Object> evaluateStatements(std::vector<std::unique_ptr<Statement>> statements);
std::unique_ptr<Object> evaluatePrefixExpr(const std::string& oprtr, const std::unique_ptr<Object>& right);
std::unique_ptr<Object> evaluateBangOperator(const std::unique_ptr<Object>& right);
std::unique_ptr<Object> evaluateMinusOperator(const std::unique_ptr<Object>& right);

} // evaluator
