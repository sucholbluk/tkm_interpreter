#define BOOST_TEST_MODULE PARSER_TESTS
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/included/unit_test.hpp>
#include <queue>
#include <sstream>

#include "parser.hpp"
#include "printer.hpp"

class MockLexer : public ILexer {
   private:
    std::deque<Token> _tokens;

   public:
    MockLexer() : _tokens{} {};
    MockLexer(std::vector<Token> tokens) : _tokens{tokens.begin(), tokens.end()} {}
    Token get_next_token() override {
        if (_tokens.empty()) {
            return Token{TokenType::T_EOF, Position{}};  // tokens have been tested - doesnt matter here
        }
        Token token{_tokens.front()};
        _tokens.pop_front();
        return token;
    }
};

BOOST_AUTO_TEST_CASE(test_constructor) {
    auto lexer = std::make_unique<MockLexer>();
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_continue) {
    std::vector<Token> tokens = {
        Token{TokenType::T_CONTINUE, Position()},
        Token(TokenType::T_SEMICOLON, Position()),
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 1);
    Printer printer{};
    program->accept(printer);
}

BOOST_AUTO_TEST_CASE(test_break) {
    std::vector<Token> tokens = {
        Token{TokenType::T_BREAK, Position()},
        Token(TokenType::T_SEMICOLON, Position()),
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 1);
    Printer printer{};
    program->accept(printer);
}

BOOST_AUTO_TEST_CASE(test_break_cont) {
    std::vector<Token> tokens = {
        Token{TokenType::T_CONTINUE, Position()},
        Token(TokenType::T_SEMICOLON, Position()),
        Token{TokenType::T_BREAK, Position()},
        Token(TokenType::T_SEMICOLON, Position()),
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 2);
    Printer printer{};
    program->accept(printer);
}

BOOST_AUTO_TEST_CASE(test_fail) {
    BOOST_CHECK_EQUAL(1, 0);
}