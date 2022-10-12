#include <gtest/gtest.h>

#include "../src/parser.h"

// Helper for detecting parsing errors
void checkParseErrors(const Parser& parser) {
    if (parser.errors().size() == 0)
        return;
    
    std::cout << "Parser had " << parser.errors().size() << " errors";

    for (const std::string& error : parser.errors())
        std::cout << "Parser error " << error << '\n';
}

TEST(ParserTest, TestLetStatements) {
    const std::string input = "let x = 9; let y = 104; let foo = 37;";

    Lexer lexer(input);
    Parser parser(lexer);

    const auto program = parser.parseProgram();
    const unsigned int n_statements = program->nStatements();

    EXPECT_TRUE(program != nullptr);
    EXPECT_EQ(n_statements, 3);

    const std::vector<std::string> expected_ident = {
        "x",
        "y",
        "foo"
    };

    for (unsigned int i = 0; i < n_statements; i++) {
        auto statement = program->getStatementAt(i);
        EXPECT_EQ(statement->tokenLiteral(), "let");
    }

    checkParseErrors(parser);
}

TEST(ParserTest, TestReturnStatements) {
    const std::string input = "return 5; return 50; return 9999; return 0;";

    Lexer lexer(input);
    Parser parser(lexer);

    const auto program = parser.parseProgram();
    const unsigned int n_statements = program->nStatements();

    checkParseErrors(parser);

    EXPECT_TRUE(program != nullptr);
    EXPECT_EQ(n_statements, 4);

    for (unsigned int i = 0; i < n_statements; i++) {
        auto statement = program->getStatementAt(i);
        EXPECT_EQ(statement->tokenLiteral(), "return");
    }
}

TEST(ParserTest, TestToString) {
    const std::string expected_str = "let test = x";
    const Token let_tok = Token(TOK_LET, "let");
    const Token first_ident_tok = Token(TOK_IDENT, "test");
    const Token second_ident_tok = Token(TOK_IDENT, "x");

    auto let_stmnt = std::make_unique<LetStatement>(let_tok);
    auto first_ident_stmnt = Identifier(first_ident_tok, "test");
    auto second_ident_stmnt = std::make_unique<Identifier>(second_ident_tok, "x");

    let_stmnt->setName(first_ident_stmnt);
    let_stmnt->setValue(std::move(second_ident_stmnt));

    auto program = std::make_unique<Program>();
    program->pushStatement(std::move(let_stmnt));

    EXPECT_EQ(program->toString(), expected_str);
}

// TODO: Add tests for Boolean and implement getValue<T>()
TEST(ParserTest, TestIdentifierExpr) {
    const std::string input = "foobar";
    Lexer lexer(input);
    Parser parser(lexer);

    const auto program = parser.parseProgram();
    checkParseErrors(parser);
    const unsigned int n_statements = program->nStatements();

    EXPECT_EQ(n_statements, 1);

    const auto ident = program->getStatementAt(0);
    
    EXPECT_EQ(ident->tokenLiteral(), "foobar");
}

TEST(ParserTest, TestIntegerLiteralExpr) {
    const std::string input = "8";
    Lexer lexer(input);
    Parser parser(lexer);

    const auto program = parser.parseProgram();
    checkParseErrors(parser);
    const unsigned int n_statements = program->nStatements();

    EXPECT_EQ(n_statements, 1);

    const auto integer = program->getStatementAt(0);
    
    EXPECT_EQ(integer->tokenLiteral(), "8");
}

TEST(ParserTest, TestParsingPrefixExpr) {
    struct PrefixTest {
        std::string input;
        std::string oprtr;
        int value;
    };

    const std::vector<PrefixTest> tests = {
        {"!3", "!", 3},
        {"-66", "-", 66}
    };

    for (const auto& test : tests) {
        Lexer lexer(test.input);
        Parser parser(lexer);

        const auto program = parser.parseProgram();
        checkParseErrors(parser);
        const unsigned int n_statements = program->nStatements();

        EXPECT_EQ(n_statements, 1);

        const auto statement = program->getStatementAt(0);
        const int num_value = stoi(statement->getExpr()->getRight()->tokenLiteral());

        EXPECT_EQ(statement->tokenLiteral(), test.oprtr);
        EXPECT_EQ(num_value, test.value);
    }
}

TEST(ParserTest, TestParsingInfixExpr) {
    struct InfixTest {
        std::string input;
        int left_val;
        std::string oprtr;
        int right_val;
    };

    const std::vector<InfixTest> tests = {
        {"1 + 2", 1, "+", 2},
        {"3 - 4", 3, "-", 4},
        {"9 * 5", 9, "*", 5},
        {"5 / 5", 5, "/", 5},
        {"1 > 2", 1, ">", 2},
        {"3 < 2", 3, "<", 2},
        {"1 == 2", 1, "==", 2},
        {"7 != 8", 7, "!=", 8},
    };

    for (const auto& test : tests) {
        Lexer lexer(test.input);
        Parser parser(lexer);

        const auto program = parser.parseProgram();
        checkParseErrors(parser);
        const unsigned int n_statements = program->nStatements();

        EXPECT_EQ(n_statements, 1);

        const auto expr = program->getStatementAt(0)->getExpr();

        EXPECT_EQ(stoi(expr->getLeft()->tokenLiteral()), test.left_val);
        EXPECT_EQ(stoi(expr->getRight()->tokenLiteral()), test.right_val);
        EXPECT_EQ(expr->tokenLiteral(), test.oprtr);
    }
}

TEST(ParserTest, TestOperatorPrecedenceParsing) {
    struct test {
        std::string input;
        std::string expected;
    };

    const std::vector<test> tests = {
        {"-a * b", "((-a) * b)"},
        {"!-a", "(!(-a))"},
        {"a + b + c", "((a + b) + c)"},
        {"a + b - c", "((a + b) - c)"},
        {"a * b * c", "((a * b) * c)"},
        {"a * b / c", "((a * b) / c)"},
        {"a + b / c", "(a + (b / c))"},
        {"a + b * c + d / e - f", "(((a + (b * c)) + (d / e)) - f)"},
        {"3 + 4; -5 * 5", "(3 + 4)((-5) * 5)"},
        {"5 > 4 == 3 < 4", "((5 > 4) == (3 < 4))"},
        {"5 < 4 != 3 < 4", "((5 < 4) != (3 < 4))"},
        {"3 + 4 * 5 == 3 * 1 + 4 * 5", "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))"},
        {"true", "true"},
        {"false", "false"},
        {"1 > 2 == false", "((1 > 2) == false)"},
        {"1 < 5 == true", "((1 < 5) == true)"},
        {"1 + (2 + 3) + 4", "((1 + (2 + 3)) + 4)"},
        {"(8 + 8) * 2", "((8 + 8) * 2)"},
        {"2 / (1 + 3)", "(2 / (1 + 3))"},
        {"-(7 + 6)", "(-(7 + 6))"},
        {"!(true == true)", "(!(true == true))"}
    };
    
    for (const auto& test: tests) {
        Lexer lexer(test.input);
        Parser parser(lexer);

        const auto program = parser.parseProgram();
        checkParseErrors(parser);

        EXPECT_EQ(program->toString(), test.expected);
    }
}

TEST(ParserTest, TestIfExprCondition) {
    const std::string input = "if (x > u) { z }";

    Lexer lexer(input);
    Parser parser(lexer);

    auto program = parser.parseProgram();
    checkParseErrors(parser);
    const unsigned int n_statements = program->nStatements();

    EXPECT_EQ(n_statements, 1);

    auto expr = program->getStatementAt(0)->getExpr();

    EXPECT_EQ(expr->getCondition()->toString(), "(x > u)");
    EXPECT_EQ(expr->getConsequence()->getStatementAt(0)->tokenLiteral(), "z");
    EXPECT_EQ(expr->getAlternative(), nullptr);
}

TEST(ParserTest, TestIfElseExpr) {
    const std::string input = "if (x > u) { x } else { y }";

    Lexer lexer(input);
    Parser parser(lexer);

    auto program = parser.parseProgram();
    checkParseErrors(parser);
    const unsigned int n_statements = program->nStatements();

    EXPECT_EQ(n_statements, 1);

    auto expr = program->getStatementAt(0)->getExpr();

    EXPECT_EQ(expr->getCondition()->toString(), "(x > u)");
    EXPECT_EQ(expr->getConsequence()->getStatementAt(0)->tokenLiteral(), "x");
    EXPECT_EQ(expr->getAlternative()->toString(), "y");
}
