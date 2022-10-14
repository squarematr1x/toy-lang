#pragma once

#include <string>
#include <memory>

enum object_type {
    OBJ_INT,
    OBJ_BOOL,
    OBJ_RETURN,
    OBJ_NIL
};

struct Object {
    std::string type;

    Object() = default;

    virtual const std::string inspect() const { return ""; }
    virtual int getType() const { return -1; }
    virtual int getIntVal() const { return 0; }
    virtual bool getBoolVal() const { return true; }

    virtual std::unique_ptr<Object> getObjValue() { return nullptr; }

    virtual ~Object() = default;
};

struct Integer : public Object {
    int value;

    Integer(int value_in) : value(value_in) {}

    const std::string inspect() const override { return std::to_string(value); }
    int getType() const override { return OBJ_INT; }

    int getIntVal() const override { return value; }
    bool getBoolVal() const override { return value; }
};

struct Bool : public Object {
    bool value;

    Bool(bool value_in) : value(value_in) {}

    const std::string inspect() const override;
    int getType() const override { return OBJ_BOOL; }

    int getIntVal() const override { return value; }
    bool getBoolVal() const override { return value; }
};

struct Return : public Object {
    std::unique_ptr<Object> value;

    Return(std::unique_ptr<Object> value_in) : value(std::move(value_in)) {}

    const std::string inspect() const override { return value->inspect(); }
    int getType() const override { return OBJ_RETURN; }

    virtual std::unique_ptr<Object> getObjValue() override { return std::move(value); }
};

struct NIL : public Object {
    const std::string inspect() const override { return "nil"; }
    int getType() const override { return OBJ_NIL; }

    bool getBoolVal() const override { return false; }
};
