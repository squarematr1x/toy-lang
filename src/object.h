#pragma once

#include <iostream>

#include "ast.h"

enum object_type {
    OBJ_INT,
    OBJ_FLOAT,
    OBJ_STR,
    OBJ_BOOL,
    OBJ_ARRAY,
    OBJ_RETURN,
    OBJ_FUNC,
    OBJ_BUILTIN,
    OBJ_NIL,
    OBJ_ERROR
};

class Env;
struct Object;

typedef std::shared_ptr<Object> ObjectPtr;

struct Object {
    std::string type;

    Object() = default;

    virtual const std::string inspect() const { return ""; }
    virtual const std::string typeString() const { return ""; }

    virtual int getType() const { return -1; }

    virtual int getIntVal() const { return 0; }
    virtual bool getBoolVal() const { return true; }
    virtual double getFloatVal() const { return -1; }
    virtual std::string getStrVal() const { return ""; }

    virtual std::shared_ptr<Object> getObjValue() { return nullptr; }
    virtual std::shared_ptr<BlockStatement> getBody() { return nullptr; }
    virtual std::weak_ptr<Env> getEnv() { return std::weak_ptr<Env>(); }

    virtual const std::vector<Identifier> getParams() const { return {}; }
    virtual std::vector<ObjectPtr> getElements() { return {}; }

    virtual std::shared_ptr<Object> clone() { return std::make_shared<Object>(*this); }

    virtual ~Object() = default;
};

struct Integer: public Object {
    int value;

    Integer(int value_in) : value(value_in) {}

    const std::string inspect() const override { return std::to_string(value); }
    const std::string typeString() const override { return "INTEGER"; }

    int getType() const override { return OBJ_INT; }

    int getIntVal() const override { return value; }
    bool getBoolVal() const override { return value; }
    double getFloatVal() const override { return static_cast<double>(value); }

    std::shared_ptr<Object> clone() override { return std::make_shared<Integer>(*this); }
};

struct Float: public Object {
    double value;

    Float(double value_in) : value(value_in) {}

    const std::string inspect() const override { return std::to_string(value); }
    const std::string typeString() const override { return "FLOAT"; }

    int getType() const override { return OBJ_FLOAT; }

    int getIntVal() const override { return static_cast<int>(value); }
    bool getBoolVal() const override { return static_cast<bool>(value); }
    double getFloatVal() const override { return value; }

    std::shared_ptr<Object> clone() override { return std::make_shared<Float>(*this); }
};

struct Bool: public Object {
    bool value;

    Bool(bool value_in) : value(value_in) {}

    const std::string inspect() const override;
    const std::string typeString() const override { return "BOOLEAN"; }

    int getType() const override { return OBJ_BOOL; }
    int getIntVal() const override { return value; }

    bool getBoolVal() const override { return value; }

    std::shared_ptr<Object> clone() override { return std::make_shared<Bool>(*this); }
};

struct String: public Object {
    std::string value;

    String(const std::string& value_in) : value(value_in) {}

    const std::string inspect() const override { return ("'" + value + "'"); }
    const std::string typeString() const override { return "STRING"; }
    std::string getStrVal() const override { return value; }

    int getType() const override { return OBJ_STR; }

    std::shared_ptr<Object> clone() override { return std::make_shared<String>(*this); }
};

struct Array: public Object {
    std::vector<ObjectPtr> elements;

    Array(std::vector<ObjectPtr> elements_in) : elements(elements_in) {}

    const std::string inspect() const override;
    const std::string typeString() const override { return "ARRAY"; }

    std::vector<ObjectPtr> getElements() override { return { elements }; }

    int getType() const override { return OBJ_ARRAY; }

    std::shared_ptr<Object> clone() override { return std::make_shared<Array>(*this); }
};

struct Return: public Object {
    std::shared_ptr<Object> value;

    Return(std::shared_ptr<Object> value_in) : value(value_in) {}

    const std::string inspect() const override { return value->inspect(); }
    const std::string typeString() const override { return "RETURN"; }

    int getType() const override { return OBJ_RETURN; }

    std::shared_ptr<Object> getObjValue() override { return value; }

    std::shared_ptr<Object> clone() override { return std::make_shared<Return>(*this); }
};

struct Function: public Object {
    std::vector<Identifier> params;
    std::shared_ptr<BlockStatement> body;
    std::weak_ptr<Env> env;

    Function(std::vector<Identifier> params, std::shared_ptr<BlockStatement> body, std::shared_ptr<Env> env)
        : params(params), body(body), env(env) {
    }

    const std::string inspect() const override;
    const std::string typeString() const override { return "FUNC"; }

    std::shared_ptr<BlockStatement> getBody() override { return body; }
    std::weak_ptr<Env> getEnv() override { return env; }

    const std::vector<Identifier> getParams() const override { return params; }

    int getType() const override { return OBJ_FUNC; }

    std::shared_ptr<Object> clone() override { return std::make_shared<Function>(*this); }
};

struct Builtin: public Object {
    std::string builtin_name;

    Builtin(const std::string builtin_name_in) : builtin_name(builtin_name_in) {}

    const std::string inspect() const override { return "builtin function"; }
    std::string getStrVal() const override { return builtin_name; }

    int getType() const override { return OBJ_BUILTIN; }

    std::shared_ptr<Object> clone() override { return std::make_shared<Builtin>(*this); }
};

struct NIL: public Object {
    const std::string inspect() const override { return "nil"; }
    const std::string typeString() const override { return "NIL"; }

    int getType() const override { return OBJ_NIL; }

    bool getBoolVal() const override { return false; }

    std::shared_ptr<Object> clone() override { return std::make_shared<NIL>(*this); }
};

struct Error: public Object {
    std::string msg;

    Error(const std::string& msg_in) : msg(msg_in) {}

    const std::string inspect() const override { return ("Error: " + msg); }
    const std::string typeString() const override { return "ERROR"; }

    int getType() const override { return OBJ_ERROR; }

    std::shared_ptr<Object> clone() override { return std::make_shared<Error>(*this); }
};
