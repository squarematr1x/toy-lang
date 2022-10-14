#include <gtest/gtest.h>

#include "../src/parser.h"
#include "../src/evaluator.h"

template <typename T>
struct EvalTest {
    std::string input;
    T expected;
};

TEST(EvaluatorTest, TestEvalIntegerExpr) {
    std::vector<EvalTest<int>> tests = {
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
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::evaluate(parser.parseProgram());

        EXPECT_EQ(obj->getIntVal(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_INT);
    }
}

TEST(EvaluatorTest, TestEvalBoolExpr) {
    std::vector<EvalTest<bool>> tests = {
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
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::evaluate(parser.parseProgram());

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
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::evaluate(parser.parseProgram());

        EXPECT_EQ(obj->getBoolVal(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_BOOL);
    }
}

TEST(EvaluatorTest, TestEvalIfElseExpr) {
    std::vector<EvalTest<int>> int_out_tests = {
        {"if (true) { 1 }", 1},
        {"if (25) { 2 }", 2},
        {"if (9 < 11) { 15 }", 15},
        {"if (3 > 5) { 10 } else { 30 }", 30},
        {"if (4 < 7) { 10 } else { 40 }", 10}
    };

    std::vector<EvalTest<std::string>> nil_out_tests = {
        {"if (false) { 10 }", "nil"},
        {"if (1 > 2) { 10 }", "nil"}
    };

    for (const auto& test : int_out_tests) {
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::evaluate(parser.parseProgram());

        EXPECT_EQ(obj->getIntVal(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_INT);
    }

    for (const auto& test : nil_out_tests) {
        Lexer lexer(test.input);
        Parser parser(lexer);
        auto obj = evaluator::evaluate(parser.parseProgram());

        EXPECT_EQ(obj->inspect(), "nil");
        EXPECT_EQ(obj->getType(), OBJ_NIL);
    }
}
