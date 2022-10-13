#pragma once

#include <string>

#include <iostream>

enum object_type {
    OBJ_INT,
    OBJ_BOOL,
    OBJ_NIL
};

struct Object {
    std::string type;

    Object() = default;

    virtual const std::string inspect() const { return ""; }
    virtual int getType() const { return -1; }
    virtual int getIVal() const { return 0; }
    virtual int getBVal() const { return 0; }

    virtual ~Object() = default;
};

struct Integer : public Object {
    int value;

    Integer(int value_in) : value(value_in) {}

    const std::string inspect() const override { return std::to_string(value); }
    int getType() const override { return OBJ_INT; }
    int getIVal() const override { return value; }
};

struct Bool : public Object {
    bool value;

    Bool(bool value_in) : value(value_in) {}

    const std::string inspect() const override { return std::to_string(value); }
    int getType() const override { return OBJ_BOOL; }

    int getBVal() const override { return value; }
};

struct NIL : public Object {
    const std::string inspect() const override { return "nil"; }
    int getType() const override { return OBJ_NIL; }
};
