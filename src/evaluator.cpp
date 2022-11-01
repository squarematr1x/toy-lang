#include "evaluator.h"
#include "builtin.h"

#include <iostream>

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
    case NODE_FLOAT:
        return std::make_shared<Float>(node->getFloatValue());
    case NODE_STR:
        return std::make_shared<String>(node->tokenLiteral());
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

        return std::make_shared<Return>(value);
    }
    case NODE_LET_STMNT: {
        auto value = eval(node->getExpr(), env);
        if (isError(value))
            return value;

        return env->set(node->getIdentName(), value);
    }
    case NODE_CALL_EXPR: {
        auto func = eval(node->getFunc(), env);
        if (isError(func))
            return func;

        auto args = evalExprs(node->getArgs(), env);
        if (args.size() == 1 && isError(args[0]))
            return args[0];
        
        return applyFunction(func, args);
    }
    case NODE_FUNC: {
        auto params = node->getParams();
        auto body = node->getBody();
        return std::make_shared<Function>(params, body, env);
    }
    case NODE_ARRAY: {
        auto elements = evalExprs(node->getElements(), env);
        if (elements.size() == 1 && isError(elements[0]))
            return elements[0];
        
        return std::make_shared<Array>(elements);
    }
    case NODE_INDEX: {
        auto left = eval(node->getLeft(), env);
        if (isError(left))
            return left;
        
        auto index = eval(node->getIndex(), env);
        if (isError(index))
            return index;
        
        return evalIndexExpr(left, index);
    }
    default:
        return std::make_shared<NIL>();
    }
}

ObjectPtr evalProgram(std::vector<std::shared_ptr<Statement>> statements, EnvPtr env) {
    std::shared_ptr<Object> result = nullptr;

    for (auto& statement: statements) {
        result = eval(statement, env);

        if (result->getType() == OBJ_RETURN)
            return result->getObjValue();
        if (result->getType() == OBJ_ERROR)
            return result;
    }
    
    return result;
}

ObjectPtr evalBlock(std::vector<std::shared_ptr<Statement>> statements, EnvPtr env) {
    std::shared_ptr<Object> result = nullptr;

    if (statements.size() == 0)
        return std::make_shared<NIL>();

    for (auto& statement: statements) {
        result = eval(statement, env);

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

    return std::make_shared<Error>(("unknown operator: " + oprtr + right->typeString()));
}

ObjectPtr evalInfixExpr(const std::string& oprtr, const ObjectPtr& left, const ObjectPtr& right) {
    if (left->getType() == OBJ_INT && right->getType() == OBJ_INT)
        return evalIntInfixExpr(oprtr, left, right);
    if (left->getType() == OBJ_INT && right->getType() == OBJ_FLOAT)
        return evalFloatInfixExpr(oprtr, left, right);
    if (left->getType() == OBJ_FLOAT && right->getType() == OBJ_INT)
        return evalFloatInfixExpr(oprtr, left, right);
    if (left->getType() == OBJ_FLOAT && right->getType() == OBJ_FLOAT)
        return evalFloatInfixExpr(oprtr, left, right);
    if (left->getType() == OBJ_STR && right->getType() == OBJ_STR)
        return evalStringInfixExpr(oprtr, left, right);
    if (oprtr == "==")
        return std::make_shared<Bool>(left->getBoolVal() == right->getBoolVal());
    if (oprtr == "!=")
        return std::make_shared<Bool>(left->getBoolVal() != right->getBoolVal());

    return std::make_shared<Error>(("unknown operator: " + left->typeString() + oprtr + right->typeString()));
}

ObjectPtr evalBangOperator(const ObjectPtr& right) {
    switch (right->getType())
    {
    case OBJ_BOOL:
        return std::make_shared<Bool>(!right->getBoolVal());
    case OBJ_NIL:
        return std::make_shared<Bool>(true);
    default:
        return std::make_shared<Bool>(false);
    }
}

ObjectPtr evalMinusOperator(const ObjectPtr& right) {
    switch (right->getType())
    {
    case OBJ_INT:
        return std::make_shared<Integer>(-right->getIntVal());
    case OBJ_FLOAT:
        return std::make_shared<Float>(-right->getFloatVal());
    default:
        return std::make_shared<Error>(("unknown operator: -" + right->typeString())); 
    }
}

ObjectPtr evalIntInfixExpr(const std::string& oprtr, const ObjectPtr& left, const ObjectPtr& right) {
    int left_value = left->getIntVal();
    int right_value = right->getIntVal();

    if (oprtr == "+")
        return std::make_shared<Integer>(left_value + right_value);
    if (oprtr == "-")
        return std::make_shared<Integer>(left_value - right_value);
    if (oprtr == "*")
        return std::make_shared<Integer>(left_value * right_value);
    if (oprtr == "/")
        return std::make_shared<Integer>(left_value / right_value);
    if (oprtr == "<")
        return std::make_shared<Bool>(left_value < right_value);
    if (oprtr == ">")
        return std::make_shared<Bool>(left_value > right_value);
    if (oprtr == "==")
        return std::make_shared<Bool>(left_value == right_value);
    if (oprtr == "!=")
        return std::make_shared<Bool>(left_value != right_value);

    return std::make_shared<Error>(("unknown operator: " + left->typeString() + oprtr + right->typeString()));
}

ObjectPtr evalFloatInfixExpr(const std::string& oprtr, const ObjectPtr& left, const ObjectPtr& right) {
    double left_value = left->getFloatVal();
    double right_value = right->getFloatVal();

    if (oprtr == "+")
        return std::make_shared<Float>(left_value + right_value);
    if (oprtr == "-")
        return std::make_shared<Float>(left_value - right_value);
    if (oprtr == "*")
        return std::make_shared<Float>(left_value * right_value);
    if (oprtr == "/")
        return std::make_shared<Float>(left_value / right_value);
    if (oprtr == "<")
        return std::make_shared<Bool>(left_value < right_value);
    if (oprtr == ">")
        return std::make_shared<Bool>(left_value > right_value);
    if (oprtr == "==")
        return std::make_shared<Bool>(left_value == right_value);
    if (oprtr == "!=")
        return std::make_shared<Bool>(left_value != right_value);

    return std::make_shared<Error>(("unknown operator: " + left->typeString() + oprtr + right->typeString()));
}

ObjectPtr evalStringInfixExpr(const std::string& oprtr, const ObjectPtr& left, const ObjectPtr& right) {
    if (oprtr != "+")
        return std::make_shared<Error>(("unknown operator: " + left->typeString() + oprtr + right->typeString()));
    
    auto left_str = left->getStrVal();
    auto right_str = right->getStrVal();

    return std::make_shared<String>(left_str + right_str);
}

ObjectPtr evalIfExpr(const ASTNodePtr& node, EnvPtr env) {
    auto cond = eval(node->getCondition(), env);
    if (isError(cond))
        return cond;

    if (isTrue(cond))
        return eval(node->getConsequence(), env);

    auto alt = node->getAlternative();
    if (alt)
        return eval(alt, env);

    return std::make_shared<NIL>();
}

ObjectPtr evalIdentifier(const ASTNodePtr& node, EnvPtr env) {
    auto value = env->get(node->getIdentName());

    if (value)
        return value;
    
    if (isBuiltIn(node->getIdentName()))
        return std::make_shared<Builtin>(node->getIdentName());

    return std::make_shared<Error>(("identifier not found: " + node->getIdentName()));
}

std::vector<ObjectPtr> evalExprs(std::vector<std::shared_ptr<Expr>> args, EnvPtr env) {
    std::vector<ObjectPtr> result;

    for (auto& arg : args) {
        auto evaluated = eval(arg, env);
        result.push_back(evaluated);

        if (isError(evaluated))
            return result;
    }

    return result;
}

ObjectPtr evalIndexExpr(const ObjectPtr& left, const ObjectPtr& index) {
    if (left->getType() == OBJ_ARRAY && index->getType() == OBJ_INT)
        return evalArrayIndexExpr(left, index);
    if (left->getType() == OBJ_STR && index->getType() == OBJ_INT)
        return evalStringIndexExpr(left, index);

    return std::make_shared<Error>(("index operator not supported: " + left->typeString()));
}

ObjectPtr evalArrayIndexExpr(const ObjectPtr& array, const ObjectPtr& index) {
    const size_t i = static_cast<size_t>(index->getIntVal());
    const size_t max = array->getElements().size() - 1;

    if (i < 0 || i > max)
        return std::make_shared<NIL>();

    return array->getElements()[i];
}

ObjectPtr evalStringIndexExpr(const ObjectPtr& str, const ObjectPtr& index) {
    const size_t i = static_cast<size_t>(index->getIntVal());
    const size_t max = str->getStrVal().length() - 1;

    if (i < 0 || i > max)
        return std::make_shared<NIL>();

    return std::make_shared<String>(std::string(1, str->getStrVal()[i]));
}

ObjectPtr applyFunction(ObjectPtr func, std::vector<ObjectPtr> args) {
    switch (func->getType())
    {
    case OBJ_FUNC: {
        const size_t n_params = func->getParams().size();
        const size_t n_args = args.size();
        if (n_params != n_args)
            return std::make_shared<Error>("wrong number of arguments. got=" + std::to_string(n_args) + ", want=" + std::to_string(n_params));

        auto extended_env = extendFunctionEnv(func, args);
        auto evaluated = evalBlock(func->getBody()->getStatements(), extended_env);

        return unwrapReturnValue(evaluated);
    }
    case OBJ_BUILTIN: {
        return getBuiltin(func->getStrVal(), args);
    }
    default:
        return std::make_shared<Error>(("not a function: " + func->typeString()));
    }
}

EnvPtr extendFunctionEnv(const ObjectPtr& func, std::vector<ObjectPtr> args) {
    auto outer_env = func->getEnv().lock();
    auto new_env = std::make_shared<Env>(outer_env);
    auto params = func->getParams();
    unsigned int i = 0;

    for (const auto& param : params) {
        new_env->set(param.getIdentName(), args[i]);
        i++;
    }

    return new_env;
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
