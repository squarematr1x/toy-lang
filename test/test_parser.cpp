#include <gtest/gtest.h>

#include "../src/parser.h"

void checkParseErrors(const Parser& parser) {
    if (parser.errors().size() == 0)
        return;
    
    std::cout << "Parser had " << parser.errors().size() << " errors";

    for (const std::string& error : parser.errors())
        std::cout << "Parser error " << error << '\n';
}

TEST(ParserTest, TestLetStatements) {
    std::string input = "let x = 9; let y = 104; let foo = 37;";
    // std::string input = "let x = 9; let = 104; let 37;";

    Lexer lexer(input);
    Parser parser(lexer);

    auto program = parser.parseProgram();
    unsigned int n_statements = program->nStatements();

    EXPECT_TRUE(program != nullptr);
    EXPECT_EQ(n_statements, 3);

    std::vector<std::string> expected_ident = {
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
    std::string input = "return 5; return 50; return 9999; return 0;";

    Lexer lexer(input);
    Parser parser(lexer);

    auto program = parser.parseProgram();
    unsigned int n_statements = program->nStatements();
    checkParseErrors(parser);

    EXPECT_TRUE(program != nullptr);
    EXPECT_EQ(n_statements, 4);

    for (unsigned int i = 0; i < n_statements; i++) {
        auto statement = program->getStatement(i);
        EXPECT_EQ(statement->tokenLiteral(), "return");
    }
}
