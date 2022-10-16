#pragma once

#include "ast.h"
#include "env.h"

namespace evaluator {

std::unique_ptr<Object> eval(const std::unique_ptr<Node>& node, Env& env);
std::unique_ptr<Object> evalProgram(std::vector<std::unique_ptr<Statement>> statements, Env& env);
std::unique_ptr<Object> evalBlock(std::vector<std::unique_ptr<Statement>> statements, Env& env);
std::unique_ptr<Object> evalPrefixExpr(const std::string& oprtr, const std::unique_ptr<Object>& right);
std::unique_ptr<Object> evalInfixExpr(const std::string& oprtr, const std::unique_ptr<Object>& left, const std::unique_ptr<Object>& right);
std::unique_ptr<Object> evalBangOperator(const std::unique_ptr<Object>& right);
std::unique_ptr<Object> evalMinusOperator(const std::unique_ptr<Object>& right);
std::unique_ptr<Object> evalIntInfixExpr(const std::string& oprtr, const std::unique_ptr<Object>& left, const std::unique_ptr<Object>& right);
std::unique_ptr<Object> evalIfExpr(const std::unique_ptr<Node>& node, Env& env);
std::unique_ptr<Object> evalIdentifier(const std::unique_ptr<Node>& node, Env& env);

std::unique_ptr<Object> error(const std::string& format);

bool isTrue(const std::unique_ptr<Object>& obj);
bool isError(const std::unique_ptr<Object>& obj);

} // evaluator
