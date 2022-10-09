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

// Helper struct
struct PrefixTests {
    std::string input;
    std::string oprtr;
    int value;
};

TEST(ParserTest, TestLetStatements) {
    const std::string input = "let x = 9; let y = 104; let foo = 37;";

    Lexer lexer(input);
    Parser parser(lexer);

    auto program = parser.parseProgram();
    const unsigned int n_statements = program->nStatements();

    EXPECT_TRUE(program != nullptr);
    EXPECT_EQ(n_statements, 3);

    const std::vector<std::string> expected_ident = {
        "x",
        "y",
        "foo"
    };

    for (unsigned int i = 0; i < n_statements; i++) {
        auto statement = program->getStatement(i);
        EXPECT_EQ(statement->tokenLiteral(), "let");
    }

    checkParseErrors(parser);
}

TEST(ParserTest, TestReturnStatements) {
    const std::string input = "return 5; return 50; return 9999; return 0;";

    Lexer lexer(input);
    Parser parser(lexer);

    auto program = parser.parseProgram();
    const unsigned int n_statements = program->nStatements();

    checkParseErrors(parser);

    EXPECT_TRUE(program != nullptr);
    EXPECT_EQ(n_statements, 4);

    for (unsigned int i = 0; i < n_statements; i++) {
        auto statement = program->getStatement(i);
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

TEST(ParserTest, TestIdentifierExpr) {
    const std::string input = "foobar";
    Lexer lexer(input);
    Parser parser(lexer);

    auto program = parser.parseProgram();
    checkParseErrors(parser);
    const unsigned int n_statements = program->nStatements();

    EXPECT_EQ(n_statements, 1);

    auto ident = program->getStatement(0);
    
    EXPECT_EQ(ident->tokenLiteral(), "foobar");
}

// TODO: check m_value here also (write methods...)
TEST(ParserTest, TestIntegerLiteralExpr) {
    const std::string input = "8";
    Lexer lexer(input);
    Parser parser(lexer);

    auto program = parser.parseProgram();
    checkParseErrors(parser);
    const unsigned int n_statements = program->nStatements();

    EXPECT_EQ(n_statements, 1);

    auto integer = program->getStatement(0);
    
    EXPECT_EQ(integer->tokenLiteral(), "8");
}

TEST(ParserTest, TestParsingPrefixExpr) {
    const std::vector<PrefixTests> tests = {
        {"!3", "!", 3},
        {"-66", "-", 66}
    };

    for (const auto& test : tests) {
        Lexer lexer(test.input);
        Parser parser(lexer);

        auto program = parser.parseProgram();
        checkParseErrors(parser);
        const unsigned int n_statements = program->nStatements();

        EXPECT_EQ(n_statements, 1);

        EXPECT_EQ(program->getStatement(0)->tokenLiteral(), test.oprtr);
        // TODO: statement->getExpr()->getRight...
    }
}
