#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
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
    {"->", TokenType::T_ARROW, Position()},
    {"==", TokenType::T_EQUAL, Position()},
    {"!=", TokenType::T_NOT_EQUAL, Position()},
    {"<=", TokenType::T_LESS_EQUAL, Position()},
    {">=", TokenType::T_GREATER_EQUAL, Position()},
    {"+", TokenType::T_PLUS, Position()},
    {"-", TokenType::T_MINUS, Position()},
    {"*", TokenType::T_MULTIPLY, Position()},
    {"/", TokenType::T_DIVIDE, Position()},
    {"=", TokenType::T_ASSIGN, Position()},
    {"(", TokenType::T_L_PAREN, Position()},
    {")", TokenType::T_R_PAREN, Position()},
    {"{", TokenType::T_L_BRACE, Position()},
    {"}", TokenType::T_R_BRACE, Position()},
    {"<", TokenType::T_LESS, Position()},
    {">", TokenType::T_GREATER, Position()},
    {"&", TokenType::T_FUNC_COMPOSITION, Position()},
    {">>", TokenType::T_BIND_FRONT, Position()},
    {";", TokenType::T_SEMICOLON, Position()},
    {":", TokenType::T_COLON, Position()},
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