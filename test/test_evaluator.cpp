#include <gtest/gtest.h>

#include "../src/parser.h"
#include "../src/evaluator.h"

TEST(EvaluatorTest, TestEvalIntegerExpr) {
    struct EvalTest {
        std::string input;
        int expected;
    };

    std::vector<EvalTest> tests = {
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

    for (const auto&  test : tests) {
        Lexer lexer(test.input);
        Parser parser(lexer);

        auto obj = evaluator::evaluate(parser.parseProgram());
        EXPECT_EQ(obj->getIVal(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_INT);
    }
}

TEST(EvaluatorTest, TestEvalBoolExpr) {
    struct EvalTest {
        std::string input;
        bool expected;
    };

    std::vector<EvalTest> tests = {
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

    for (const auto&  test : tests) {
        Lexer lexer(test.input);
        Parser parser(lexer);

        auto obj = evaluator::evaluate(parser.parseProgram());
        EXPECT_EQ(obj->getBVal(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_BOOL);
    }
}

TEST(EvaluatorTest, TestEvanBangOperator) {
    struct EvalTest {
        std::string input;
        bool expected;
    };

    const std::vector<EvalTest> tests = {
        {"!false", true},
        {"!2", false},
        {"!!true", true},
        {"!!false", false},
        {"!!!true", false},
        {"!!7", true}
    };

    for (const auto&  test : tests) {
        Lexer lexer(test.input);
        Parser parser(lexer);

        auto obj = evaluator::evaluate(parser.parseProgram());
        EXPECT_EQ(obj->getBVal(), test.expected);
        EXPECT_EQ(obj->getType(), OBJ_BOOL);
    }
}
