#include "evaluator.h"

namespace evaluator {

std::unique_ptr<Object> eval(const std::unique_ptr<Node>& node, Env& env) {
    switch (node->nodeType())
    {
    case NODE_PROGRAM:
        return evalProgram(node->getStatements(), env);
    case NODE_EXPR_STMNT:
        return eval(node->getExpr(), env);
    case NODE_IDENT:
        return evalIdentifier(node, env);
    case NODE_INT:   
        return std::make_unique<Integer>(node->getIntValue());
    case NODE_BOOL:
        return std::make_unique<Bool>(node->getBoolValue());
    case NODE_PREFIX: {
        auto right = eval(node->getRight(), env);
        if (isError(right))
            return right;

        return evalPrefixExpr(node->tokenLiteral(), right);
    }
    case NODE_INFIX: {
        auto left = eval(node->getLeft(), env);
        if (isError(left))
            return left;
    
        auto right = eval(node->getRight(), env);
        if (isError(right))
            return right;

        return evalInfixExpr(node->tokenLiteral(), left, right);
    }
    case NODE_BLOCK_STMNT:
        return evalBlock(node->getStatements(), env);
    case NODE_IF_EXPR:
        return evalIfExpr(node, env);
    case NODE_RETURN_STMNT: {
        auto value = eval(node->getExpr(), env);
        if (isError(value))
            return value;

        return std::make_unique<Return>(std::move(value));
    }
    case NODE_LET_STMNT: {
        auto value = eval(node->getExpr(), env);
        if (isError(value))
            return value;

        env.set(node->getIdentName(), std::move(value));
    }
    default:
        return std::make_unique<NIL>();
    }
}

std::unique_ptr<Object> evalProgram(std::vector<std::unique_ptr<Statement>> statements, Env& env) {
    std::unique_ptr<Object> result = nullptr;

    for (auto& statement: statements) {
        result = eval(std::move(statement), env);

        if (result->getType() == OBJ_RETURN)
            return result->getObjValue();
        if (result->getType() == OBJ_ERROR)
            return result;
    }
    
    return result;
}

std::unique_ptr<Object> evalBlock(std::vector<std::unique_ptr<Statement>> statements, Env& env) {
    std::unique_ptr<Object> result = nullptr;

    for (auto& statement: statements) {
        result = eval(std::move(statement), env);

        if (result->getType() == OBJ_RETURN || result->getType() == OBJ_ERROR)
            return result;
    }
    
    return result;
}

std::unique_ptr<Object> evalPrefixExpr(const std::string& oprtr, const std::unique_ptr<Object>& right) {
    if (oprtr == "!")
        return evalBangOperator(right);
    if (oprtr == "-")
        return evalMinusOperator(right);

    return std::make_unique<Error>(("unknown operator: " + oprtr + right->typeString()));
}

std::unique_ptr<Object> evalInfixExpr(const std::string& oprtr, const std::unique_ptr<Object>& left, const std::unique_ptr<Object>& right) {
    if (left->getType() == OBJ_INT && right->getType() == OBJ_INT)
        return evalIntInfixExpr(oprtr, left, right);
    if (oprtr == "==")
        return std::make_unique<Bool>(left->getBoolVal() == right->getBoolVal());
    if (oprtr == "!=")
        return std::make_unique<Bool>(left->getBoolVal() != right->getBoolVal());

    return std::make_unique<Error>(("unknown operator: " + left->typeString() + oprtr + right->typeString()));
}

std::unique_ptr<Object> evalBangOperator(const std::unique_ptr<Object>& right) {
    switch (right->getType())
    {
    case OBJ_BOOL:
        return std::make_unique<Bool>(!right->getBoolVal());
    case OBJ_NIL:
        return std::make_unique<Bool>(true);
    default:
        return std::make_unique<Bool>(false);
    }
}

std::unique_ptr<Object> evalMinusOperator(const std::unique_ptr<Object>& right) {
    if (right->getType() != OBJ_INT)
        return std::make_unique<Error>(("unknown operator: -" + right->typeString()));
    
    int value = right->getIntVal();

    return std::make_unique<Integer>(-value);
}

std::unique_ptr<Object> evalIntInfixExpr(const std::string& oprtr, const std::unique_ptr<Object>& left, const std::unique_ptr<Object>& right) {
    int left_value = left->getIntVal();
    int right_value = right->getIntVal();

    if (oprtr == "+")
        return std::make_unique<Integer>(left_value + right_value);
    if (oprtr == "-")
        return std::make_unique<Integer>(left_value - right_value);
    if (oprtr == "*")
        return std::make_unique<Integer>(left_value * right_value);
    if (oprtr == "/")
        return std::make_unique<Integer>(left_value / right_value);
    if (oprtr == "<")
        return std::make_unique<Bool>(left_value < right_value);
    if (oprtr == ">")
        return std::make_unique<Bool>(left_value > right_value);
    if (oprtr == "==")
        return std::make_unique<Bool>(left_value == right_value);
    if (oprtr == "!=")
        return std::make_unique<Bool>(left_value != right_value);

    return std::make_unique<Error>(("unknown operator: " + left->typeString() + oprtr + right->typeString()));
}

std::unique_ptr<Object> evalIfExpr(const std::unique_ptr<Node>& node, Env& env) {
    auto cond = eval(node->getCondition(), env);
    if (isError(cond))
        return cond;

    if (isTrue(cond))
        return eval(node->getConsequence(), env);

    auto alt = node->getAlternative();
    if (alt)
        return eval(std::move(alt), env);

    return std::make_unique<NIL>();
}

std::unique_ptr<Object> evalIdentifier(const std::unique_ptr<Node>& node, Env& env) {
    auto value = env.get(node->getIdentName());

    if (!value)
        return std::make_unique<Error>(("identifier not found: " + node->getIdentName()));
    
    return value;
}

bool isTrue(const std::unique_ptr<Object>& obj) {
    return obj->getBoolVal();
}

bool isError(const std::unique_ptr<Object>& obj) {
    if (obj)
        return obj->getType() == OBJ_ERROR;
    
    return false;
}

} // evaluator
