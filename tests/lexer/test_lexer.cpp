#define BOOST_TEST_MODULE LEXER_TESTS
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/test/unit_test.hpp>

#include "lexer.hpp"

namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_CASE(constructor_test) {
    std::stringstream source{"+"};
    auto handler = std::make_unique<SourceHandler>(source);

    Lexer lexer{std::move(handler)};
    Token plus = lexer.get_next_token();
    Token eof = lexer.get_next_token();
    BOOST_CHECK(plus.get_type()._to_integral() == TokenType::T_PLUS);
    BOOST_CHECK_EQUAL(plus.get_position(), Position());

    BOOST_CHECK(eof.get_type()._to_integral() == TokenType::T_EOF);
    BOOST_CHECK_EQUAL(eof.get_position(), Position(1, 2));
}

std::vector<std::tuple<std::string, TokenType, Position>> simple_operator_test_cases{
    {"->", TokenType::T_ARROW, Position{}},
    {"==", TokenType::T_EQUAL, Position{}},
    {"!=", TokenType::T_NOT_EQUAL, Position{}},
    {"<=", TokenType::T_LESS_EQUAL, Position{}},
    {">=", TokenType::T_GREATER_EQUAL, Position{}},
    {"+", TokenType::T_PLUS, Position{}},
    {"-", TokenType::T_MINUS, Position{}},
    {"*", TokenType::T_MULTIPLY, Position{}},
    {"/", TokenType::T_DIVIDE, Position{}},
    {"=", TokenType::T_ASSIGN, Position{}},
    {"(", TokenType::T_L_PAREN, Position{}},
    {")", TokenType::T_R_PAREN, Position{}},
    {"{", TokenType::T_L_BRACE, Position{}},
    {"}", TokenType::T_R_BRACE, Position{}},
    {"<", TokenType::T_LESS, Position{}},
    {">", TokenType::T_GREATER, Position{}},
    {"&", TokenType::T_FUNC_COMPOSITION, Position{}},
    {">>", TokenType::T_BIND_FRONT, Position{}},
    {";", TokenType::T_SEMICOLON, Position{}},
    {":", TokenType::T_COLON, Position{}},
    {"_example_variable", TokenType::T_IDENTIFIER, Position{}},
    {"int", TokenType::T_INT, Position{}},
    {"float", TokenType::T_FLOAT, Position{}},
    {"bool", TokenType::T_BOOL, Position{}},
    {"string", TokenType::T_STRING, Position{}},
    {"function", TokenType::T_FUNCTION, Position{}},
    {"none", TokenType::T_NONE, Position{}},
    {"not", TokenType::T_NOT, Position{}},
    {"and", TokenType::T_AND, Position{}},
    {"or", TokenType::T_OR, Position{}},
    {"def", TokenType::T_DEF, Position{}},
    {"let", TokenType::T_LET, Position{}},
    {"mut", TokenType::T_MUT, Position{}},
    {"as", TokenType::T_AS, Position{}},
    {"if", TokenType::T_IF, Position{}},
    {"else", TokenType::T_ELSE, Position{}},
    {"for", TokenType::T_FOR, Position{}},
    {"break", TokenType::T_BREAK, Position{}},
    {"continue", TokenType::T_CONTINUE, Position{}},
    {"return", TokenType::T_RETURN, Position{}},
    {"true", TokenType::T_TRUE, Position{}},
    {"false", TokenType::T_FALSE, Position{}},
};

BOOST_DATA_TEST_CASE(simple_operator_tests, bdata::make(simple_operator_test_cases), input, type, position) {
    std::stringstream source{input};
    auto handler = std::make_unique<SourceHandler>(source);

    Lexer lexer{std::move(handler)};
    Token oper = lexer.get_next_token();
    Token eof = lexer.get_next_token();

    BOOST_CHECK_EQUAL(oper.get_type(), type);
    BOOST_CHECK_EQUAL(oper.get_position(), position);
    BOOST_CHECK(eof.get_type()._to_integral() == TokenType::T_EOF);
}