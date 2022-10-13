#include "evaluator.h"

#include <iostream>

namespace evaluator {

std::unique_ptr<Object> evaluate(const std::unique_ptr<Node>& node) {
    switch (node->nodeType())
    {
    case NODE_PROGRAM:
        return evalStatements(node->getStatements());
    case NODE_EXPR_STMNT:
        return evaluate(node->getExpr());
    case NODE_INT:   
        return std::make_unique<Integer>(node->getIntValue());
    case NODE_BOOL:
        return std::make_unique<Bool>(node->getBoolValue());
    case NODE_PREFIX: {
        auto right = evaluate(node->getRight());
        return evalPrefixExpr(node->tokenLiteral(), right);
    }
    case NODE_INFIX: {
        auto left = evaluate(node->getLeft());
        auto right = evaluate(node->getRight());
        return evalInfixExpr(node->tokenLiteral(), left, right);
    }
    default:
        return std::make_unique<NIL>();
    }
}

std::unique_ptr<Object> evalStatements(std::vector<std::unique_ptr<Statement>> statements) {
    std::unique_ptr<Object> result = nullptr;

    for (auto& statement: statements) {
        result = evaluate(std::move(statement));
    }
    
    return result;
}

std::unique_ptr<Object> evalPrefixExpr(const std::string& oprtr, const std::unique_ptr<Object>& right) {
    if (oprtr == "!")
        return evalBangOperator(right);
    if (oprtr == "-")
        return evalMinusOperator(right);

    return std::make_unique<NIL>();
}

std::unique_ptr<Object> evalInfixExpr(const std::string& oprtr, const std::unique_ptr<Object>& left, const std::unique_ptr<Object>& right) {
    if (left->getType() == OBJ_INT && right->getType() == OBJ_INT)
        return evalIntInfixExpr(oprtr, left, right);
    if (oprtr == "==")
        return std::make_unique<Bool>(left->getBVal() == right->getBVal());
    if (oprtr == "!=")
        return std::make_unique<Bool>(left->getBVal() != right->getBVal());

    return std::make_unique<NIL>();
}

std::unique_ptr<Object> evalBangOperator(const std::unique_ptr<Object>& right) {
    switch (right->getType())
    {
    case OBJ_BOOL:
        return std::make_unique<Bool>(!right->getBVal());
    case OBJ_NIL: {
        return std::make_unique<Bool>(true);
    }
    default:
        return std::make_unique<Bool>(false);
    }
}

std::unique_ptr<Object> evalMinusOperator(const std::unique_ptr<Object>& right) {
    if (right->getType() != OBJ_INT)
        return std::make_unique<NIL>();
    
    int value = right->getIVal();

    return std::make_unique<Integer>(-value);
}

std::unique_ptr<Object> evalIntInfixExpr(const std::string& oprtr, const std::unique_ptr<Object>& left, const std::unique_ptr<Object>& right) {
    int left_value = left->getIVal();
    int right_value = right->getIVal();

    if (oprtr == "+")
        return std::make_unique<Integer>(left_value + right_value);
    else if (oprtr == "-")
        return std::make_unique<Integer>(left_value - right_value);
    else if (oprtr == "*")
        return std::make_unique<Integer>(left_value * right_value);
    else if (oprtr == "/")
        return std::make_unique<Integer>(left_value / right_value);
    else if (oprtr == "<")
        return std::make_unique<Bool>(left_value < right_value);
    else if (oprtr == ">")
        return std::make_unique<Bool>(left_value > right_value);
    else if (oprtr == "==")
        return std::make_unique<Bool>(left_value == right_value);
    else if (oprtr == "!=")
        return std::make_unique<Bool>(left_value != right_value);
    else
        return std::make_unique<NIL>();
}

} // evaluator
