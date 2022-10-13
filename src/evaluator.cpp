#include "evaluator.h"

#include <iostream>

namespace evaluator {

std::unique_ptr<Object> evaluate(const std::unique_ptr<Node>& node) {
    switch (node->nodeType())
    {
    case NODE_PROGRAM:
        return evaluateStatements(node->getStatements());
    case NODE_EXPR_STMNT:
        return evaluate(node->getExpr());
    case NODE_INT:   
        return std::make_unique<Integer>(node->getIntValue());
    case NODE_BOOL:
        return std::make_unique<Bool>(node->getBoolValue());
    case NODE_PREFIX: {
        auto right = evaluate(node->getRight());
        return evaluatePrefixExpr(node->tokenLiteral(), right);
    }
    default:
        return std::make_unique<NIL>();
    }
}

std::unique_ptr<Object> evaluateStatements(std::vector<std::unique_ptr<Statement>> statements) {
    std::unique_ptr<Object> result = nullptr;

    for (auto& statement: statements) {
        result = evaluate(std::move(statement));
    }
    
    return result;
}

std::unique_ptr<Object> evaluatePrefixExpr(const std::string& oprtr, const std::unique_ptr<Object>& right) {
    if (oprtr == "!")
        return evaluateBangOperator(right);
    if (oprtr == "-")
        return evaluateMinusOperator(right);

    return std::make_unique<NIL>();
}

std::unique_ptr<Object> evaluateBangOperator(const std::unique_ptr<Object>& right) {
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

std::unique_ptr<Object> evaluateMinusOperator(const std::unique_ptr<Object>& right) {
    if (right->getType() != OBJ_INT)
        return std::make_unique<NIL>();
    
    int value = right->getIVal();

    return std::make_unique<Integer>(-value);
}

} // evaluator
