#pragma once

#include <iostream>

#include "ast.h"
#include "env.h"

namespace evaluator {

ObjectPtr eval(const ASTNodePtr& node, EnvPtr env);
ObjectPtr evalProgram(std::vector<std::shared_ptr<Statement>> statements, EnvPtr env);
ObjectPtr evalBlock(std::vector<std::shared_ptr<Statement>> statements, EnvPtr env);
ObjectPtr evalPrefixExpr(const std::string& oprtr, const ObjectPtr& right);
ObjectPtr evalInfixExpr(const std::string& oprtr, const ObjectPtr& left, const ObjectPtr& right);
ObjectPtr evalBangOperator(const ObjectPtr& right);
ObjectPtr evalMinusOperator(const ObjectPtr& right);
ObjectPtr evalIntInfixExpr(const std::string& oprtr, const ObjectPtr& left, const ObjectPtr& right);
ObjectPtr evalFloatInfixExpr(const std::string& oprtr, const ObjectPtr& left, const ObjectPtr& right);
ObjectPtr evalStringInfixExpr(const std::string& oprtr, const ObjectPtr& left, const ObjectPtr& right);
ObjectPtr evalIfExpr(const ASTNodePtr& node, EnvPtr env);
ObjectPtr evalIdentifier(const ASTNodePtr& node, EnvPtr env);
ObjectPtr evalIndexExpr(const ObjectPtr& left, const ObjectPtr& index);
ObjectPtr evalArrayIndexExpr(const ObjectPtr& array, const ObjectPtr& index);
ObjectPtr evalStringIndexExpr(const ObjectPtr& str, const ObjectPtr& index);
ObjectPtr evalHashIndexExpr(const ObjectPtr& hash, const ObjectPtr& index);
ObjectPtr evalHashLiteral(const ASTNodePtr& node, EnvPtr env);

ObjectPtr applyFunction(ObjectPtr func, std::vector<ObjectPtr> args);
ObjectPtr unwrapReturnValue(ObjectPtr obj);

ObjectPtr error(const std::string& format);

std::vector<ObjectPtr> evalExprs(std::vector<ExprPtr> args, EnvPtr env);

EnvPtr extendFunctionEnv(const ObjectPtr& func, std::vector<ObjectPtr> args);

bool isTrue(const ObjectPtr& obj);
bool isError(const ObjectPtr& obj);

} // evaluator
