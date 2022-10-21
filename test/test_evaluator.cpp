#include <gtest/gtest.h>

#include "../src/parser.h"
#include "../src/evaluator.h"

template <typename T>
struct EvalTest {
    std::string input;
    T expected;
};

TEST(EvaluatorTest, TestEvalIntegerExpr) {
    const std::vector<EvalTest<int>> tests = {
        {"5", 5},
        {"17", 17},
        {"-99", -99},
        {"-4", -4},
        {"5 + 5 + 3 + 5 - 10", 8},
        {"2 * 2 * 2 * 2 * 2", 32},
        {"-50 + 100 + -50", 0},
        {"3 * 3 + 10", 19},
        {"1 + 2 * 10", 21},
        {"20 + 2 * -10", 0},
        {"50 / 2 * 2 + 10", 60},
        {"2 * (5 + 10)", 30},
        {"3 * 3 * 3 + 10", 37},
        {"3 * (3 * 3) + 10", 37},
        {"(5 + 10 * 2 + 15 / 3) * 2 + -10", 50},
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

TEST(EvaluatorTest, TestEvalStringLiteral) {
    const std::string input = "\"Some string blaa\"";
    EnvPtr env = std::make_shared<Env>();
    Lexer lexer(input);
    Parser parser(lexer);

    auto obj = evaluator::eval(parser.parseProgram(), env);

    EXPECT_EQ(obj->getStrVal(), "Some string blaa");
    EXPECT_EQ(obj->getType(), OBJ_STR);
}

TEST(EvaluatorTest, TestEvalBoolExpr) {
    const std::vector<EvalTest<bool>> tests = {
        {"true", true},
        {"false", false},
        {"0 < 2", true},
        {"2 > 4", false},
        {"1 < 1", false},
        {"3 > 3", false},
        {"1 == 1", true},
        {"5 != 5", false},
        {"1 == 2", false},
        {"1 != 5", true},
        {"true == true", true},
        {"false == false", true},
        {"true == false", false},
        {"true != false", true},
        {"false != true", true},
        {"(11 < 55) == true", true},
        {"(5 < 2) == true", false},
        {"(11 > 55) == false", true},
        {"(5 > 2) == true", true},
    };

    for (const auto& test : tests) {
        EnvPtr env = std::make_shared<Env>();
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::eval(parser.parseProgram(), env);

        EXPECT_EQ(obj->getBoolVal(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_BOOL);
    }
}

TEST(EvaluatorTest, TestEvalBangOperator) {
    const std::vector<EvalTest<bool>> tests = {
        {"!false", true},
        {"!2", false},
        {"!!true", true},
        {"!!false", false},
        {"!!!true", false},
        {"!!7", true}
    };

    for (const auto& test : tests) {
        EnvPtr env = std::make_shared<Env>();
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::eval(parser.parseProgram(), env);

        EXPECT_EQ(obj->getBoolVal(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_BOOL);
    }
}

TEST(EvaluatorTest, TestEvalIfElseExpr) {
    const std::vector<EvalTest<int>> int_out_tests = {
        {"if (true) { 1 }", 1},
        {"if (25) { 2 }", 2},
        {"if (9 < 11) { 15 }", 15},
        {"if (3 > 5) { 10 } else { 30 }", 30},
        {"if (4 < 7) { 10 } else { 40 }", 10}
    };

    const std::vector<EvalTest<std::string>> nil_out_tests = {
        {"if (false) { 10 }", "nil"},
        {"if (1 > 2) { 10 }", "nil"}
    };

    for (const auto& test : int_out_tests) {
        EnvPtr env = std::make_shared<Env>();
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::eval(parser.parseProgram(), env);

        EXPECT_EQ(obj->getIntVal(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_INT);
    }

    for (const auto& test : nil_out_tests) {
        EnvPtr env = std::make_shared<Env>();
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::eval(parser.parseProgram(), env);

        EXPECT_EQ(obj->inspect(), "nil");
        EXPECT_EQ(obj->getType(), OBJ_NIL);
    }
}

TEST(EvaluatorTest, TestEvalReturnStatements) {
    const std::vector<EvalTest<int>> tests = {
        {"return 1;", 1},
        {"return 88; 2;", 88},
        {"return 3 * 4; 11;", 12},
        {"5; return 7 * 7; 3;", 49},
        {"if (9 > 3) { if (8 > 2) { return 10; } return 2; }", 10},
        {"if (3 > 54) { if (8 > 2) { return 10; }} else {return 2; }", 2}
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

TEST(EvaluatorTest, TestErrorHandling) {
    const std::vector<EvalTest<std::string>> tests = {
        {"-true", "Error: unknown operator: -BOOLEAN"},
        {"true + false;", "Error: unknown operator: BOOLEAN+BOOLEAN"},
        {"5; true + false; 5", "Error: unknown operator: BOOLEAN+BOOLEAN"},
        {"if (10 > 1) { true + false; }","Error: unknown operator: BOOLEAN+BOOLEAN"},
        {"if (10 > 1) { if (10 > 1) { return true + false; } return 1; }", "Error: unknown operator: BOOLEAN+BOOLEAN"},
        {"some_var", "Error: identifier not found: some_var"},
        {"\"first str\" - \"second str\"", "Error: unknown operator: STRING-STRING"}
    };

    for (const auto& test : tests) {
        EnvPtr env = std::make_shared<Env>();
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::eval(parser.parseProgram(), env);

        EXPECT_EQ(obj->getType(), OBJ_ERROR);
        EXPECT_EQ(obj->inspect(), test.expected);
    }
}

TEST(EvaluatorTest, TestEvalLetStatements) {
    const std::vector<EvalTest<int>> tests = {
        {"let x = 4; x;", 4},
        {"let x = 2*3; x;", 6},
        {"let foo = 3;  let bar = foo; bar;", 3},
        {"let x = 12; let y = x; let z = x + y + 1; z;", 25}
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

TEST(EvaluatorTest, TestEvalFunctionObject) {
    const std::string input = "func(a) { a + 21; };";

    EnvPtr env;
    Lexer lexer(input);
    Parser parser(lexer);
    auto obj = evaluator::eval(parser.parseProgram(), env);

    EXPECT_EQ(obj->getType(), OBJ_FUNC);
    EXPECT_EQ(obj->getParams().size(), 1);
    EXPECT_EQ(obj->getParams()[0].getIdentName(), "a");
    EXPECT_EQ(obj->getBody()->toString(), "(a + 21)");
}

TEST(EvaluatorTest, TestEvalFunctionApp) {
    const std::vector<EvalTest<int>> tests = {
        {"let test = func() { return -12; }; test();", -12},
        {"let echo = func(x) { x; }; echo(7);", 7},
        {"let echo = func(x) { return x; }; echo(7);", 7},
        {"let twice = func(x) { return x*2; }; twice(7);", 14},
        {"let add = func(a, b) { a + b; }; add(5, 4);", 9},
        {"let add = func(a, b) { a + b; }; add(5 + 4, add(1, 2));", 12},
        {"func(x) { x; }(8);", 8},
        {"let newAdder = func(x) { func(y) { x + y }; }; let addTwo = newAdder(2); addTwo(2);", 4},
        {"let a = func(x) { x*x; }; let b = func(x) { if (x > 10) { 1; } else {  0; } }; b(a(4));", 1},
        {"let a = func(x) { x*x; }; let b = func(x) { if (x > 10) { return 1; } else {  return 0; } }; b(a(3));", 0}
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

TEST(EvaluatorTest, TestStringConcatenation) {
    const std::string input = "\"How are\" + \" you?\"";
    EnvPtr env;
    Lexer lexer(input);
    Parser parser(lexer);
    auto obj = evaluator::eval(parser.parseProgram(), env);

    EXPECT_EQ(obj->getType(), OBJ_STR);
    EXPECT_EQ(obj->getStrVal(), "How are you?");
}   

TEST(EvaluatorTest, TestBuiltinFunctions) {
    const std::vector<EvalTest<int>> tests = {
        {"len(\"\")", 0},
        {"len(\"test\")", 4},
        {"len(\"Hello world!\")", 12}
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

TEST(EvaluatorTest, TestBuiltinFunctionErrors) {
    const std::vector<EvalTest<std::string>> tests = {
        {"len(4)", "Error: argument 'len' not supported, got=INTEGER"},
        {"len(\"first\", \"second\")", "Error: wrong number of arguments. got=2, want=1"},
        {"len()", "Error: wrong number of arguments. got=0, want=1"}
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
