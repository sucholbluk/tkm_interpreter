#define BOOST_TEST_MODULE LEXER_TESTS
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>

#include "lexer.hpp"
#include "token_type.hpp"

namespace bdata = boost::unit_test::data;
using namespace tkm;
BOOST_AUTO_TEST_CASE(constructor_test) {
    std::unique_ptr<std::istream> source = std::make_unique<std::stringstream>("+");
    auto handler = std::make_unique<SourceHandler>(std::move(source));

    Lexer lexer{std::move(handler)};
    Token plus{lexer.get_next_token()};
    Token eof{lexer.get_next_token()};
    BOOST_CHECK(plus.get_type() == TokenType::T_PLUS);
    BOOST_CHECK_EQUAL(plus.get_position(), Position());

    BOOST_CHECK(eof.get_type() == TokenType::T_EOF);
    BOOST_CHECK_EQUAL(eof.get_position(), Position(1, 2));
}

BOOST_AUTO_TEST_CASE(bool_test) {
    std::unique_ptr<std::istream> source = std::make_unique<std::stringstream>("true false");
    auto handler = std::make_unique<SourceHandler>(std::move(source));

    Lexer lexer{std::move(handler)};
    Token t_true{lexer.get_next_token()};
    Token t_false{lexer.get_next_token()};
    Token eof{lexer.get_next_token()};

    BOOST_CHECK(t_true.get_type() == TokenType::T_LITERAL_BOOL);
    BOOST_CHECK_EQUAL(t_true.get_value_as<bool>(), true);
    BOOST_CHECK_EQUAL(t_true.get_position(), Position());

    BOOST_CHECK(t_false.get_type() == TokenType::T_LITERAL_BOOL);
    BOOST_CHECK_EQUAL(t_false.get_value_as<bool>(), false);
    BOOST_CHECK_EQUAL(t_false.get_position(), Position(1, 6));

    BOOST_CHECK(eof.get_type() == TokenType::T_EOF);
    BOOST_CHECK_EQUAL(eof.get_position(), Position(1, 11));
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
    {"true", TokenType::T_LITERAL_BOOL},
    {"false", TokenType::T_LITERAL_BOOL},
    {"123456789", TokenType::T_LITERAL_INT},
    {"123456789.1232392323283998493", TokenType::T_LITERAL_FLOAT},
    {"# here is a comment", TokenType::T_COMMENT},
    {"\"Here is the string$#@!@!er3343\"", TokenType::T_LITERAL_STRING},
    {"\"string with space after that should not be tokenized\"   ", TokenType::T_LITERAL_STRING}

};

BOOST_DATA_TEST_CASE(simple_tests, bdata::make(simple_test_cases), input, type) {
    std::unique_ptr<std::istream> source = std::make_unique<std::stringstream>(input);
    auto handler = std::make_unique<SourceHandler>(std::move(source));

    Lexer lexer{std::move(handler)};
    Token tk{lexer.get_next_token()};
    Token eof{lexer.get_next_token()};

    BOOST_CHECK_EQUAL(tk.get_type(), type);
    BOOST_CHECK_EQUAL(tk.get_position(), Position{});
    BOOST_CHECK(eof.get_type() == TokenType::T_EOF);
}

std::vector<std::tuple<std::string, std::string>> string_test_cases{
    {"\"\\\"string\\\" \\nwith \\tescape\\\\s\"", "\"string\" \nwith \tescape\\s"},
    {"\"string with spaces\"", "string with spaces"},
    {"\"special characters !@#$%^&*()\"", "special characters !@#$%^&*()"},
    {"\"string with trailing spaces   \"", "string with trailing spaces   "},
};

BOOST_DATA_TEST_CASE(string_values_test, bdata::make(string_test_cases), input, expected_value) {
    std::unique_ptr<std::istream> source = std::make_unique<std::stringstream>(input);
    auto handler = std::make_unique<SourceHandler>(std::move(source));
    Lexer lexer{std::move(handler)};
    Token str{lexer.get_next_token()};
    Token eof{lexer.get_next_token()};

    BOOST_CHECK_EQUAL(str.get_type(), TokenType::T_LITERAL_STRING);
    BOOST_CHECK_EQUAL(str.get_value_as<std::string>(), expected_value);
    BOOST_CHECK_EQUAL(str.get_position(), Position{});
}

std::vector<std::tuple<std::string, int>> int_test_cases{
    {"0", 0},
    {"123", 123},
    {"456789", 456789},
    {"2147483647", 2147483647},
};

BOOST_DATA_TEST_CASE(int_tests, bdata::make(int_test_cases), input, expected_value) {
    std::unique_ptr<std::istream> source = std::make_unique<std::stringstream>(input);
    auto handler = std::make_unique<SourceHandler>(std::move(source));

    Lexer lexer{std::move(handler)};
    Token tk = lexer.get_next_token();

    BOOST_CHECK_EQUAL(tk.get_type(), TokenType::T_LITERAL_INT);
    BOOST_CHECK_EQUAL(tk.get_value_as<int>(), expected_value);

    Token eof = lexer.get_next_token();
    BOOST_CHECK_EQUAL(eof.get_type(), TokenType::T_EOF);
}

std::vector<std::tuple<std::string, double>> float_test_cases{
    {"0.0", 0.0},
    {"123.25", 123.25},
    {"3.141592", 3.141592},
    {"12312.456222222", 12312.456222222},
    {"78912.0191233232", 78912.0191233232},
    {"31212.14159", 31212.14159},
    {"120222.500", 120222.500},
    {"2147483647.18446744073709551615", 2147483647.18446744073709551615}};

BOOST_DATA_TEST_CASE(float_tests, bdata::make(float_test_cases), input, expected_value) {
    std::unique_ptr<std::istream> source = std::make_unique<std::stringstream>(input);
    auto handler = std::make_unique<SourceHandler>(std::move(source));

    Lexer lexer{std::move(handler)};
    Token tk = lexer.get_next_token();

    BOOST_CHECK_EQUAL(tk.get_type(), TokenType::T_LITERAL_FLOAT);
    BOOST_CHECK_CLOSE(tk.get_value_as<double>(), expected_value, 0.001);

    Token eof = lexer.get_next_token();
    BOOST_CHECK_EQUAL(eof.get_type(), TokenType::T_EOF);
}

BOOST_AUTO_TEST_CASE(fibbonacci_test) {
    std::string mock_file = R"EOF(
def nth_fibonacci(n: int) -> int { #some comment here
    if (n <= 1) {
        return n;
    }

    return nth_fibonacci(n - 1) + nth_fibonacci(n - 2);
}

let n: int = 5;

print("For " + n as string + " sequence number is " + nth_fibonacci(n) as string);
)EOF";
    std::unique_ptr<std::istream> source = std::make_unique<std::stringstream>(mock_file);
    auto handler = std::make_unique<SourceHandler>(std::move(source));
    Lexer lexer{std::move(handler)};
    std::vector<Token> expected_tokens = {
        Token{TokenType::T_DEF, Position(2, 1)},
        Token{TokenType::T_IDENTIFIER, Position(2, 5), "nth_fibonacci"},
        Token{TokenType::T_L_PAREN, Position(2, 18)},
        Token{TokenType::T_IDENTIFIER, Position(2, 19), "n"},
        Token{TokenType::T_COLON, Position(2, 20)},
        Token{TokenType::T_INT, Position(2, 22)},
        Token{TokenType::T_R_PAREN, Position(2, 25)},
        Token{TokenType::T_ARROW, Position(2, 27)},
        Token{TokenType::T_INT, Position(2, 30)},
        Token{TokenType::T_L_BRACE, Position(2, 34)},
        Token{TokenType::T_COMMENT, Position(2, 36), "some comment here"},
        Token{TokenType::T_IF, Position(3, 5)},
        Token{TokenType::T_L_PAREN, Position(3, 8)},
        Token{TokenType::T_IDENTIFIER, Position(3, 9), "n"},
        Token{TokenType::T_LESS_EQUAL, Position(3, 11)},
        Token{TokenType::T_LITERAL_INT, Position(3, 14), 1},
        Token{TokenType::T_R_PAREN, Position(3, 15)},
        Token{TokenType::T_L_BRACE, Position(3, 17)},
        Token{TokenType::T_RETURN, Position(4, 9)},
        Token{TokenType::T_IDENTIFIER, Position(4, 16), "n"},
        Token{TokenType::T_SEMICOLON, Position(4, 17)},
        Token{TokenType::T_R_BRACE, Position(5, 5)},
        Token{TokenType::T_RETURN, Position(7, 5)},
        Token{TokenType::T_IDENTIFIER, Position(7, 12), "nth_fibonacci"},
        Token{TokenType::T_L_PAREN, Position(7, 25)},
        Token{TokenType::T_IDENTIFIER, Position(7, 26), "n"},
        Token{TokenType::T_MINUS, Position(7, 28)},
        Token{TokenType::T_LITERAL_INT, Position(7, 30), 1},
        Token{TokenType::T_R_PAREN, Position(7, 31)},
        Token{TokenType::T_PLUS, Position(7, 33)},
        Token{TokenType::T_IDENTIFIER, Position(7, 35), "nth_fibonacci"},
        Token{TokenType::T_L_PAREN, Position(7, 48)},
        Token{TokenType::T_IDENTIFIER, Position(7, 49), "n"},
        Token{TokenType::T_MINUS, Position(7, 51)},
        Token{TokenType::T_LITERAL_INT, Position(7, 53), 2},
        Token{TokenType::T_R_PAREN, Position(7, 54)},
        Token{TokenType::T_SEMICOLON, Position(7, 55)},
        Token{TokenType::T_R_BRACE, Position(8, 1)},
        Token{TokenType::T_LET, Position(10, 1)},
        Token{TokenType::T_IDENTIFIER, Position(10, 5), "n"},
        Token{TokenType::T_COLON, Position(10, 6)},
        Token{TokenType::T_INT, Position(10, 8)},
        Token{TokenType::T_ASSIGN, Position(10, 12)},
        Token{TokenType::T_LITERAL_INT, Position(10, 14), 5},
        Token{TokenType::T_SEMICOLON, Position(10, 15)},
        Token{TokenType::T_IDENTIFIER, Position(12, 1), "print"},
        Token{TokenType::T_L_PAREN, Position(12, 6)},
        Token{TokenType::T_LITERAL_STRING, Position(12, 7), "For "},
        Token{TokenType::T_PLUS, Position(12, 14)},
        Token{TokenType::T_IDENTIFIER, Position(12, 16), "n"},
        Token{TokenType::T_AS, Position(12, 18)},
        Token{TokenType::T_STRING, Position(12, 21)},
        Token{TokenType::T_PLUS, Position(12, 28)},
        Token{TokenType::T_LITERAL_STRING, Position(12, 30), " sequence number is "},
        Token{TokenType::T_PLUS, Position(12, 53)},
        Token{TokenType::T_IDENTIFIER, Position(12, 55), "nth_fibonacci"},
        Token{TokenType::T_L_PAREN, Position(12, 68)},
        Token{TokenType::T_IDENTIFIER, Position(12, 69), "n"},
        Token{TokenType::T_R_PAREN, Position(12, 70)},
        Token{TokenType::T_AS, Position(12, 72)},
        Token{TokenType::T_STRING, Position(12, 75)},
        Token{TokenType::T_R_PAREN, Position(12, 81)},
        Token{TokenType::T_SEMICOLON, Position(12, 82)},
        Token{TokenType::T_EOF, Position(13, 1)},
    };

    for (const auto& expected_token : expected_tokens) {
        Token actual_token = lexer.get_next_token();
        BOOST_CHECK_EQUAL(actual_token.get_type(), expected_token.get_type());
        BOOST_CHECK_EQUAL(actual_token.get_position(), expected_token.get_position());

        // sample code doesnt have any bool or float
        switch (actual_token.get_type()) {
            case TokenType::T_IDENTIFIER:
            case TokenType::T_LITERAL_STRING:
            case TokenType::T_COMMENT:
                BOOST_CHECK_EQUAL(actual_token.get_value_as<std::string>(), expected_token.get_value_as<std::string>());
                break;
            case TokenType::T_LITERAL_INT:
                BOOST_CHECK_EQUAL(actual_token.get_value_as<int>(), expected_token.get_value_as<int>());
            default:
                break;
        }
    }
}

BOOST_AUTO_TEST_CASE(unfinished_string_test) {
    std::unique_ptr<std::istream> source = std::make_unique<std::stringstream>("\"feferererdfhjdf7^ # ");
    auto handler = std::make_unique<SourceHandler>(std::move(source));
    Lexer lexer{std::move(handler)};

    BOOST_CHECK_THROW(lexer.get_next_token(), UnfinishedStringException);
}
BOOST_AUTO_TEST_CASE(newline_interrupted_string_test) {
    std::unique_ptr<std::istream> source = std::make_unique<std::stringstream>("\"erdfhjd\nf7^ # \"");
    auto handler = std::make_unique<SourceHandler>(std::move(source));
    Lexer lexer{std::move(handler)};

    BOOST_CHECK_THROW(lexer.get_next_token(), UnfinishedStringException);
}

BOOST_AUTO_TEST_CASE(to_large_int_test) {
    std::unique_ptr<std::istream> source = std::make_unique<std::stringstream>("1234567890123");
    auto handler = std::make_unique<SourceHandler>(std::move(source));
    Lexer lexer{std::move(handler)};

    BOOST_CHECK_THROW(lexer.get_next_token(), ParseIntOverflowException);
}

BOOST_AUTO_TEST_CASE(to_many_fraction_nums) {
    std::unique_ptr<std::istream> source = std::make_unique<std::stringstream>("1.999999999999999999999");
    auto handler = std::make_unique<SourceHandler>(std::move(source));
    Lexer lexer{std::move(handler)};

    BOOST_CHECK_THROW(lexer.get_next_token(), ParseFractionRangeExceededException);
}

BOOST_AUTO_TEST_CASE(unexpected_character) {
    std::unique_ptr<std::istream> source = std::make_unique<std::stringstream>("let mut x: @int = 8;");
    auto handler = std::make_unique<SourceHandler>(std::move(source));
    Lexer lexer{std::move(handler)};

    for (int i = 0; i < 4; i++) {
        lexer.get_next_token();
    }

    BOOST_CHECK_THROW(lexer.get_next_token(), UnexpectedCharacterException);
}
