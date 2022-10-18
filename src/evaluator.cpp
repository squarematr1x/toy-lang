#include "evaluator.h"

namespace evaluator {

ObjectPtr eval(const ASTNodePtr& node, EnvPtr env) {
    switch (node->nodeType())
    {
    case NODE_PROGRAM:
        return evalProgram(node->getStatements(), env);
    case NODE_EXPR_STMNT:
        return eval(node->getExpr(), env);
    case NODE_IDENT:
        return evalIdentifier(node, env);
    case NODE_INT:   
        return std::make_shared<Integer>(node->getIntValue());
    case NODE_BOOL:
        return std::make_shared<Bool>(node->getBoolValue());
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

        return std::make_shared<Return>(std::move(value));
    }
    case NODE_LET_STMNT: {
        auto value = eval(node->getExpr(), env);
        if (isError(value))
            return value;

        return env->set(node->getIdentName(), std::move(value));
    }
    case NODE_CALL_EXPR: {
        auto func = eval(node->getFunc(), env);
        if (isError(func))
            return func;
        
        auto args = evalExprs(node->getArgs(), env);
        if (args.size() == 1 && isError(args[0]))
            return std::move(args[0]);
        
        return applyFunction(std::move(func), std::move(args));
    }
    case NODE_FUNC: {
        auto params = node->getParams();
        auto body = node->getBody();
        return std::make_shared<Function>(params, std::move(body), env);
    }
    default:
        return std::make_shared<NIL>();
    }
}

ObjectPtr evalProgram(std::vector<std::unique_ptr<Statement>> statements, EnvPtr env) {
    std::shared_ptr<Object> result = nullptr;

    for (auto& statement: statements) {
        result = eval(std::move(statement), env);

        if (result->getType() == OBJ_RETURN)
            return result->getObjValue();
        if (result->getType() == OBJ_ERROR)
            return result;
    }
    
    return result;
}

ObjectPtr evalBlock(std::vector<std::unique_ptr<Statement>> statements, EnvPtr env) {
    std::shared_ptr<Object> result = nullptr;

    for (auto& statement: statements) {
        result = eval(std::move(statement), env);

        if (result->getType() == OBJ_RETURN || result->getType() == OBJ_ERROR)
            return result;
    }
    
    return result;
}

ObjectPtr evalPrefixExpr(const std::string& oprtr, const ObjectPtr& right) {
    if (oprtr == "!")
        return evalBangOperator(right);
    if (oprtr == "-")
        return evalMinusOperator(right);

    return std::make_unique<Error>(("unknown operator: " + oprtr + right->typeString()));
}

ObjectPtr evalInfixExpr(const std::string& oprtr, const ObjectPtr& left, const ObjectPtr& right) {
    if (left->getType() == OBJ_INT && right->getType() == OBJ_INT)
        return evalIntInfixExpr(oprtr, left, right);
    if (oprtr == "==")
        return std::make_unique<Bool>(left->getBoolVal() == right->getBoolVal());
    if (oprtr == "!=")
        return std::make_unique<Bool>(left->getBoolVal() != right->getBoolVal());

    return std::make_unique<Error>(("unknown operator: " + left->typeString() + oprtr + right->typeString()));
}

ObjectPtr evalBangOperator(const ObjectPtr& right) {
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

ObjectPtr evalMinusOperator(const ObjectPtr& right) {
    if (right->getType() != OBJ_INT)
        return std::make_unique<Error>(("unknown operator: -" + right->typeString()));
    
    int value = right->getIntVal();

    return std::make_unique<Integer>(-value);
}

ObjectPtr evalIntInfixExpr(const std::string& oprtr, const ObjectPtr& left, const ObjectPtr& right) {
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

ObjectPtr evalIfExpr(const ASTNodePtr& node, EnvPtr env) {
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

ObjectPtr evalIdentifier(const ASTNodePtr& node, EnvPtr env) {
    if (!node)
        std::cout << "NOT NODE\n";

    if (node->getIdentName() == "")
        std::cout << "IDENT NAME = ''n";

    auto value = env->get(node->getIdentName());

    if (!value)
        return std::make_unique<Error>(("identifier not found: " + node->getIdentName()));
    
    return value;
}

std::vector<ObjectPtr> evalExprs(std::vector<std::unique_ptr<Expr>> args, EnvPtr env) {
    std::vector<ObjectPtr> result;

    for (auto& arg : args) {
        auto evaluated = eval(std::move(arg), env);
        result.push_back(std::move(evaluated));

        if (isError(evaluated))
            return result;
    }

    return result;
}

ObjectPtr applyFunction(ObjectPtr func, std::vector<ObjectPtr> args) {
    if (func->getType() != OBJ_FUNC)
        return std::make_unique<Error>(("not a function: " + func->typeString()));
    
    auto extended_env = extendFunctionEnv(func->getParams(), std::move(args), func->getEnv());
    // auto evaluated = eval(func->getBody(), extended_env);
    auto evaluated = evalBlock(func->getBody()->getStatements(), extended_env);

    return unwrapReturnValue(std::move(evaluated));
}


EnvPtr extendFunctionEnv(std::vector<Identifier> params, std::vector<ObjectPtr> args, EnvPtr env) {
    unsigned int i = 0;
    for (const auto& param : params) {
        env->set(param.getIdentName(), std::move(args[i]));
        i++;
    }

    return env;
}

ObjectPtr unwrapReturnValue(ObjectPtr obj) {
    if (obj->getType() == OBJ_RETURN)
        return obj->getObjValue();
    
    return obj;
}

bool isTrue(const ObjectPtr& obj) {
    return obj->getBoolVal();
}

bool isError(const ObjectPtr& obj) {
    if (obj)
        return obj->getType() == OBJ_ERROR;
    
    return false;
}

} // evaluator
