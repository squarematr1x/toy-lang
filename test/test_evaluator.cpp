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
        {"-4", -4}
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
        {"false", false}
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
