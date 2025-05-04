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
            return Token{TokenType::T_EOF, Position{}};  // tokens have been tested -  position doesnt matter here
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

// continue;
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

// break;
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

// continue;
// break;
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

// return 4;
BOOST_AUTO_TEST_CASE(test_return_literal_int) {
    std::vector<Token> tokens = {
        Token{TokenType::T_RETURN, Position()},
        Token(TokenType::T_LITERAL_INT, Position(), 4),
        Token(TokenType::T_SEMICOLON, Position()),
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 1);
    Printer printer{};
    program->accept(printer);
}

// return (4.5);
BOOST_AUTO_TEST_CASE(test_return_paren) {
    std::vector<Token> tokens = {
        Token{TokenType::T_RETURN, Position()},
        Token{TokenType::T_L_PAREN, Position()},
        Token(TokenType::T_LITERAL_FLOAT, Position(), 4.5),
        Token{TokenType::T_R_PAREN, Position()},
        Token(TokenType::T_SEMICOLON, Position()),
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 1);
    Printer printer{};
    program->accept(printer);
}

// return (3 * 5) / divisor;
BOOST_AUTO_TEST_CASE(test_return_paren_multiplication) {
    std::vector<Token> tokens = {
        Token{TokenType::T_RETURN, Position()},         Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 3}, Token{TokenType::T_MULTIPLY, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 5}, Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_DIVIDE, Position()},         Token{TokenType::T_IDENTIFIER, Position(), "divisor"},
        Token{TokenType::T_SEMICOLON, Position()},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 1);
    Printer printer{};
    program->accept(printer);
}
// let is_foo: bool = (foo())("Hello World")
BOOST_AUTO_TEST_CASE(test_variable_declaration_with_function_call) {
    std::vector<Token> tokens = {
        Token{TokenType::T_LET, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "is_foo"},
        Token{TokenType::T_COLON, Position()},
        Token{TokenType::T_BOOL, Position()},
        Token{TokenType::T_ASSIGN, Position()},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "foo"},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_LITERAL_STRING, Position(), "Hello World"},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_SEMICOLON, Position()},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    Printer printer{};
    program->accept(printer);
}

// {
//  let inc_42: function<mut int: none> = (42) >> inc_by;
//  return inc_42;
// }
BOOST_AUTO_TEST_CASE(test_parse_code_block) {
    std::vector<Token> tokens = {
        Token{TokenType::T_L_BRACE, Position()},
        Token{TokenType::T_LET, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "inc_42"},
        Token{TokenType::T_COLON, Position()},
        Token{TokenType::T_FUNCTION, Position()},
        Token{TokenType::T_LESS, Position()},
        Token{TokenType::T_MUT, Position()},
        Token{TokenType::T_INT, Position()},
        Token{TokenType::T_COLON, Position()},
        Token{TokenType::T_NONE, Position()},
        Token{TokenType::T_GREATER, Position()},
        Token{TokenType::T_ASSIGN, Position()},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 42},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_BIND_FRONT, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "inc_by"},
        Token{TokenType::T_SEMICOLON, Position()},
        Token{TokenType::T_RETURN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "inc_42"},
        Token{TokenType::T_SEMICOLON, Position()},
        Token{TokenType::T_R_BRACE, Position()},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 1);
    Printer printer{};
    program->accept(printer);
}
// if (x < 4){
//     return x + 2;
// } else if (x > 5) {
//     return x - 3;
// } else {
//     return x * 2;
// }
BOOST_AUTO_TEST_CASE(test_if_else_statement) {
    std::vector<Token> tokens = {
        Token{TokenType::T_IF, Position()},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_LESS, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 4},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_L_BRACE, Position()},
        Token{TokenType::T_RETURN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_PLUS, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 2},
        Token{TokenType::T_SEMICOLON, Position()},
        Token{TokenType::T_R_BRACE, Position()},
        Token{TokenType::T_ELSE, Position()},
        Token{TokenType::T_IF, Position()},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_GREATER, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 5},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_L_BRACE, Position()},
        Token{TokenType::T_RETURN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_MINUS, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 3},
        Token{TokenType::T_SEMICOLON, Position()},
        Token{TokenType::T_R_BRACE, Position()},
        Token{TokenType::T_ELSE, Position()},
        Token{TokenType::T_L_BRACE, Position()},
        Token{TokenType::T_RETURN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_MULTIPLY, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 2},
        Token{TokenType::T_SEMICOLON, Position()},
        Token{TokenType::T_R_BRACE, Position()},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    Printer printer{};
    program->accept(printer);
}

// some_num = (4.5 + 8 as float) * 2.5 + sqrt(3) * 1.5 / 9 as float;
BOOST_AUTO_TEST_CASE(test_assignment_with_complex_expression_and_cast) {
    std::vector<Token> tokens = {
        Token{TokenType::T_IDENTIFIER, Position(), "some_num"},
        Token{TokenType::T_ASSIGN, Position()},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_LITERAL_FLOAT, Position(), 4.5},
        Token{TokenType::T_PLUS, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 8},
        Token{TokenType::T_AS, Position()},
        Token{TokenType::T_FLOAT, Position()},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_MULTIPLY, Position()},
        Token{TokenType::T_LITERAL_FLOAT, Position(), 2.5},
        Token{TokenType::T_PLUS, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "sqrt"},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 3},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_MULTIPLY, Position()},
        Token{TokenType::T_LITERAL_FLOAT, Position(), 1.5},
        Token{TokenType::T_DIVIDE, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 9},
        Token{TokenType::T_AS, Position()},
        Token{TokenType::T_FLOAT, Position()},
        Token{TokenType::T_SEMICOLON, Position()},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    Printer printer{};
    program->accept(printer);
}

// (is_primary(x) or x <= 2) and x > 0 or "hello" + " world" == "hello world";

BOOST_AUTO_TEST_CASE(test_complex_logical_and_arithmetic_expression) {
    std::vector<Token> tokens = {
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "is_primary"},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_OR, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_LESS_EQUAL, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 2},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_AND, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_GREATER, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 0},
        Token{TokenType::T_OR, Position()},
        Token{TokenType::T_LITERAL_STRING, Position(), "hello"},
        Token{TokenType::T_PLUS, Position()},
        Token{TokenType::T_LITERAL_STRING, Position(), " world"},
        Token{TokenType::T_EQUAL, Position()},
        Token{TokenType::T_LITERAL_STRING, Position(), "hello world"},
        Token{TokenType::T_SEMICOLON, Position()},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    Printer printer{};
    program->accept(printer);
}

BOOST_AUTO_TEST_CASE(test_fail) {
    BOOST_CHECK_EQUAL(1, 0);
}