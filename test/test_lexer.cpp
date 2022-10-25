#include <gtest/gtest.h>
#include <vector>

#include "../src/lexer.h"

TEST(LexerTest, TestNextToken) {
    const std::string input = "let x = 10; \"blaah\"; let y = 3; let foo = func(x, y) { x + y * 3/2 - 4;}; if (x > 5) return true else return false a == b a != b 2 < !z \"some string\" \"hello\"";
    Lexer lexer(input);

    const std::vector<Token> test_tokens = {
        {TOK_LET, "let"},
        {TOK_IDENT, "x"},
        {TOK_ASSIGN, "="},
        {TOK_INT, "10"},
        {TOK_SEMICOLON, ";"},
        {TOK_STR, "blaah"},
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
        {TOK_STR, "some string"},
        {TOK_STR, "hello"},
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

TEST(LexerTest, TestGetNumberToken) {
    std::vector<std::pair<std::string, int>> tests = {
        {"4.51", TOK_FLOAT},
        {"1.1", TOK_FLOAT},
        {"101551.0", TOK_FLOAT},
        {"04124.430", TOK_FLOAT},
        {"6236264", TOK_INT},
        {"0", TOK_INT},
        {"153", TOK_INT},
        {"1636126.", TOK_ILLEGAL},
        {".02124", TOK_ILLEGAL},
        {"24.21.0", TOK_ILLEGAL},
    };

    Lexer lexer("");
    
    for (const auto& test : tests) { 
        const Token tok = lexer.getNumberToken(test.first);
        EXPECT_EQ(tok.literal, test.first);
        EXPECT_EQ(tok.type, test.second);
    }
}
