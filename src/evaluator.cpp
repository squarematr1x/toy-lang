#include "evaluator.h"

#include <iostream>

namespace evaluator {

std::unique_ptr<Object> evaluate(const std::unique_ptr<Node>& node) {
    switch (node->nodeType())
    {
    case NODE_PROGRAM:
        return evalProgram(node->getStatements());
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
    case NODE_BLOCK_STMNT:
        return evalBlock(node->getStatements());
    case NODE_IF_EXPR:
        return evalIfExpr(node);
    case NODE_RETURN_STMNT: {
        auto value = evaluate(node->getExpr());
        return std::make_unique<Return>(std::move(value));
    }
    default:
        return std::make_unique<NIL>();
    }
}

std::unique_ptr<Object> evalProgram(std::vector<std::unique_ptr<Statement>> statements) {
    std::unique_ptr<Object> result = nullptr;

    for (auto& statement: statements) {
        result = evaluate(std::move(statement));

        if (result->getType() == OBJ_RETURN)
            return result->getObjValue();
    }
    
    return result;
}

std::unique_ptr<Object> evalBlock(std::vector<std::unique_ptr<Statement>> statements) {
    std::unique_ptr<Object> result = nullptr;

    for (auto& statement: statements) {
        result = evaluate(std::move(statement));

        if (result->getType() == OBJ_RETURN)
            return result;
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
        return std::make_unique<Bool>(left->getBoolVal() == right->getBoolVal());
    if (oprtr == "!=")
        return std::make_unique<Bool>(left->getBoolVal() != right->getBoolVal());

    return std::make_unique<NIL>();
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
        return std::make_unique<NIL>();
    
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

    return std::make_unique<NIL>();
}

std::unique_ptr<Object> evalIfExpr(const std::unique_ptr<Node>& node) {
    if (isTrue(evaluate(node->getCondition())))
        return evaluate(node->getConsequence());

    auto alt = node->getAlternative();
    if (alt)
        return evaluate(std::move(alt));

    return std::make_unique<NIL>();
}

bool isTrue(const std::unique_ptr<Object>& obj) {
    return obj->getBoolVal();
}

} // evaluator