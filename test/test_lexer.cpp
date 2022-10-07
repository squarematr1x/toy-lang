#include <gtest/gtest.h>
#include <vector>

#include "../src/lexer.h"

TEST(LexerTest, TestNextToken) {
    const std::string input = "let x = 10; let y = 3; let foo = func(x, y) { x + y * 3/2 - 4;}; if (x > 5) return true else return false a == b a != b 2 < !z";
    Lexer lexer(input);

    const std::vector<Token> test_tokens = {
        {TOK_LET, "let"},
        {TOK_IDENT, "x"},
        {TOK_ASSIGN, "="},
        {TOK_INT, "10"},
        {TOK_SEMICOLON, ";"},
        {TOK_LET, "let"},
        {TOK_IDENT, "y"},
        {TOK_ASSIGN, "="},
        {TOK_INT, "3"},
        {TOK_SEMICOLON, ";"},
        {TOK_LET, "let"},
        {TOK_IDENT, "foo"},
        {TOK_ASSIGN, "="},
        {TOK_FUNC, "func"},
        {TOK_LPAREN, "("},
        {TOK_IDENT, "x"},
        {TOK_COMMA, ","},
        {TOK_IDENT, "y"},
        {TOK_RPAREN, ")"},
        {TOK_LBRACE, "{"},
        {TOK_IDENT, "x"},
        {TOK_PLUS, "+"},
        {TOK_IDENT, "y"},
        {TOK_MUL, "*"},
        {TOK_INT, "3"},
        {TOK_DIV, "/"},
        {TOK_INT, "2"},
        {TOK_MINUS, "-"},
        {TOK_INT, "4"},
        {TOK_SEMICOLON, ";"},
        {TOK_RBRACE, "}"},
        {TOK_SEMICOLON, ";"},
        {TOK_IF, "if"},
        {TOK_LPAREN, "("},
        {TOK_IDENT, "x"},
        {TOK_GT, ">"},
        {TOK_INT, "5"},
        {TOK_RPAREN, ")"},
        {TOK_RETURN, "return"},
        {TOK_TRUE, "true"},
        {TOK_ELSE, "else"},
        {TOK_RETURN, "return"},
        {TOK_FALSE, "false"},
        {TOK_IDENT, "a"},
        {TOK_EQ, "=="},
        {TOK_IDENT, "b"},
        {TOK_IDENT, "a"},
        {TOK_NOT_EQ, "!="},
        {TOK_IDENT, "b"},
        {TOK_INT, "2"},
        {TOK_LT, "<"},
        {TOK_BANG, "!"},
        {TOK_IDENT, "z"},
        {TOK_EOF, ""}
    };

    for (const auto& test_tok : test_tokens) {
        Token tok = lexer.nextToken();

        EXPECT_EQ(tok.type, test_tok.type);
        EXPECT_EQ(tok.literal, tok.literal);
    }
}

TEST(LexerTest, TestEmptyInputLexer) {
    const std::string input = "";
    Lexer lexer(input);
    const Token expected_tok = {TOK_EOF, ""};
    Token tok = lexer.nextToken();

    EXPECT_EQ(tok.type, expected_tok.type);
    EXPECT_EQ(tok.literal, expected_tok.literal);
}
