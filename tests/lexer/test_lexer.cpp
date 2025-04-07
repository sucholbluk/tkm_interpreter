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

std::vector<std::tuple<std::string, TokenType>> simple_test_cases{
    {"->", TokenType::T_ARROW},
    {"==", TokenType::T_EQUAL},
    {"!=", TokenType::T_NOT_EQUAL},
    {"<=", TokenType::T_LESS_EQUAL},
    {">=", TokenType::T_GREATER_EQUAL},
    {"+", TokenType::T_PLUS},
    {"-", TokenType::T_MINUS},
    {"*", TokenType::T_MULTIPLY},
    {"/", TokenType::T_DIVIDE},
    {"=", TokenType::T_ASSIGN},
    {"(", TokenType::T_L_PAREN},
    {")", TokenType::T_R_PAREN},
    {"{", TokenType::T_L_BRACE},
    {"}", TokenType::T_R_BRACE},
    {"<", TokenType::T_LESS},
    {">", TokenType::T_GREATER},
    {"&", TokenType::T_FUNC_COMPOSITION},
    {">>", TokenType::T_BIND_FRONT},
    {";", TokenType::T_SEMICOLON},
    {":", TokenType::T_COLON},
    {"_example_variable", TokenType::T_IDENTIFIER},
    {"int", TokenType::T_INT},
    {"float", TokenType::T_FLOAT},
    {"bool", TokenType::T_BOOL},
    {"string", TokenType::T_STRING},
    {"function", TokenType::T_FUNCTION},
    {"none", TokenType::T_NONE},
    {"not", TokenType::T_NOT},
    {"and", TokenType::T_AND},
    {"or", TokenType::T_OR},
    {"def", TokenType::T_DEF},
    {"let", TokenType::T_LET},
    {"mut", TokenType::T_MUT},
    {"as", TokenType::T_AS},
    {"if", TokenType::T_IF},
    {"else", TokenType::T_ELSE},
    {"for", TokenType::T_FOR},
    {"break", TokenType::T_BREAK},
    {"continue", TokenType::T_CONTINUE},
    {"return", TokenType::T_RETURN},
    {"true", TokenType::T_TRUE},
    {"false", TokenType::T_FALSE},
    {"123456789", TokenType::T_LITERAL_INT},
    {"123456789.1232392323283998493", TokenType::T_LITERAL_FLOAT},

};

BOOST_DATA_TEST_CASE(simple_tests, bdata::make(simple_test_cases), input, type) {
    std::stringstream source{input};
    auto handler = std::make_unique<SourceHandler>(source);

    Lexer lexer{std::move(handler)};
    Token tk = lexer.get_next_token();
    Token eof = lexer.get_next_token();

    std::cout << tk << std::endl;
    std::cout << eof << std::endl;

    BOOST_CHECK_EQUAL(tk.get_type(), type);
    BOOST_CHECK_EQUAL(tk.get_position(), Position{});
    BOOST_CHECK(eof.get_type()._to_integral() == TokenType::T_EOF);
}