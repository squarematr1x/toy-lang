#include <gtest/gtest.h>

#include "../src/parser.h"
#include "../src/evaluator.h"

template <typename T>
struct BuiltinTest {
    std::string input;
    T expected;
};

TEST(BuiltinTest, TestBuiltinLen) {
    const std::vector<BuiltinTest<int>> tests = {
        {"len(\"\")", 0},
        {"len(\"test\")", 4},
        {"len(\"Hello world!\")", 12},
        {"let arr = [1, 3, true, 5]; len(arr)", 4},
        {"len([0.45, 2, 33, 1, 21])", 5},
        {"let arr = []; len(arr)", 0},
        {"len([])", 0}
    };

    for (const auto& test : tests) {
        EnvPtr env = std::make_shared<Env>();
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::eval(parser.parseProgram(), env);
        EXPECT_EQ(obj->getIntVal(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_INT);
    }
}

TEST(BuiltinTest, TestBuiltinFirstLast) {
    const std::vector<BuiltinTest<int>> tests = {
        {"first([3, 5, 9])", 3},
        {"last([3, 5, 9])", 9},
        {"let someArray = [124, 13, 15]; first(someArray)", 124},
        {"let someArray = [124, 13, 15]; last(someArray)", 15}
    };

    for (const auto& test : tests) {
        EnvPtr env = std::make_shared<Env>();
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::eval(parser.parseProgram(), env);
        EXPECT_EQ(obj->getIntVal(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_INT);
    }
}

TEST(BuiltinTest, TestBuiltinPush) {
    const std::vector<BuiltinTest<int>> tests_a = {
        {"let a = [false, 2, 3.4]; let b = push(a, 4); len(a)", 3},
        {"let a = [false, 2, 3.4]; let b = push(a, 4); len(b)", 4}
    };

    const std::vector<BuiltinTest<std::string>> tests_b = {
        {"let a = [2*2, \"hello\", true, [4, 5]]; let b = push(a, 9); a", "[4, 'hello', true, [4, 5]]"},
        {"let a = [2*2, \"hello\", true, [4, 5]]; let b = push(a, 9); b", "[4, 'hello', true, [4, 5], 9]"}
    };

    for (const auto& test : tests_a) {
        EnvPtr env = std::make_shared<Env>();
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::eval(parser.parseProgram(), env);
        EXPECT_EQ(obj->getIntVal(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_INT);
    }

    for (const auto& test : tests_b) {
        EnvPtr env = std::make_shared<Env>();
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::eval(parser.parseProgram(), env);
        EXPECT_EQ(obj->inspect(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_ARRAY);
    }
}

TEST(BuiltinTest, TestBuiltinType) {
    const std::vector<BuiltinTest<std::string>> tests = {
        {"type(25)", "'INTEGER'"},
        {"type(5.2)", "'FLOAT'"},
        {"type(false)", "'BOOLEAN'"},
        {"type(\"yes yes\")", "'STRING'"},
        {"type(func() { return 1; })", "'FUNC'"},
        {"type([25, 1.2, \"hi\"])", "'ARRAY'"},
        {"let test = 5.25; type(test)", "'FLOAT'"}
    };

    for (const auto& test : tests) {
        EnvPtr env = std::make_shared<Env>();
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::eval(parser.parseProgram(), env);
        EXPECT_EQ(obj->getStrVal(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_STR);
    }
}

TEST(BuiltinTest, TestBuiltinPrint) {
    const std::vector<BuiltinTest<std::string>> tests = {
        {"print()", ""},
        {"print(25)", "25"},
        {"print(\"this is: \", true)", "this is: true"},
        {"print([1, 2 , 3], \" is an array\")", "[1, 2, 3] is an array"},
        {"print(5.4, \" \", false, \" \", [1, 2])", "5.400000 false [1, 2]"},
        {"let test = \"Hello world!\"; print(test)", "Hello world!"},
        {"let test = \"Hello world!\"; print(test[1], test[2])", "el"},
        {"print(len([1, 2, 3, 4]))", "4"}
    };

    for (const auto& test : tests) {
        EnvPtr env = std::make_shared<Env>();
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::eval(parser.parseProgram(), env);
        EXPECT_EQ(obj->getStrVal(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_STR);
    }
}

TEST(BuiltinTest, TestBuiltinFunctionErrors) {
    const std::vector<BuiltinTest<std::string>> tests = {
        {"len(4)", "Error: argument 'len' not supported, got=INTEGER"},
        {"len(\"first\", \"second\")", "Error: wrong number of arguments. got=2, want=1"},
        {"len()", "Error: wrong number of arguments. got=0, want=1"},
        {"len([1,2], [14, 4, 3])", "Error: wrong number of arguments. got=2, want=1"},
        {"first(15+2)", "Error: argument to 'first' must be ARRAY, got=INTEGER"},
        {"first(true)", "Error: argument to 'first' must be ARRAY, got=BOOLEAN"},
        {"first()", "Error: wrong number of arguments. got=0, want=1"},
        {"first(2, 5.5)", "Error: wrong number of arguments. got=2, want=1"},
        {"last(15+2)", "Error: argument to 'last' must be ARRAY, got=INTEGER"},
        {"last(true)", "Error: argument to 'last' must be ARRAY, got=BOOLEAN"},
        {"last(2, 24, 5.5)", "Error: wrong number of arguments. got=3, want=1"},
        {"push(5.2, a)", "Error: argument to 'push' must be ARRAY, got=FLOAT"},
        {"push([1, 2, true], 2, 4)", "Error: wrong number of arguments. got=3, want=2"},
        {"type()", "Error: wrong number of arguments. got=0, want=1"},
        {"type(2, 5.4)", "Error: wrong number of arguments. got=2, want=1"},
        {"print(func() { return 1; })", "Error: can't print object of type=FUNC"}
    };

    for (const auto& test : tests) {
        EnvPtr env = std::make_shared<Env>();
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::eval(parser.parseProgram(), env);
        EXPECT_EQ(obj->inspect(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_ERROR);
    }
}
