#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>
#include <tuple>

#include "token.hpp"

namespace bdata = boost::unit_test::data;

std::vector<std::tuple<TokenType, Position>> non_value_tokens_test_cases{
    {TokenType::T_AND, Position{1, 1}},
    {TokenType::T_INT, Position{2, 3}},
    {TokenType::T_FLOAT, Position{4, 5}},
    {TokenType::T_BOOL, Position{6, 7}},
    {TokenType::T_STRING, Position{8, 9}},
    {TokenType::T_FUNCTION, Position{10, 11}},
    {TokenType::T_NONE, Position{12, 13}},
    {TokenType::T_PLUS, Position{14, 15}},
    {TokenType::T_MINUS, Position{16, 17}},
    {TokenType::T_MULTIPLY, Position{18, 19}},
    {TokenType::T_DIVIDE, Position{20, 21}},
    {TokenType::T_ASSIGN, Position{22, 23}},
    {TokenType::T_EQUAL, Position{24, 25}},
    {TokenType::T_NOT_EQUAL, Position{26, 27}},
    {TokenType::T_LESS_EQUAL, Position{28, 29}},
    {TokenType::T_GREATER_EQUAL, Position{30, 31}},
    {TokenType::T_LESS, Position{32, 33}},
    {TokenType::T_GREATER, Position{34, 35}},
    {TokenType::T_BIND_FRONT, Position{36, 37}},
    {TokenType::T_FUNC_COMPOSITION, Position{38, 39}},
};

BOOST_DATA_TEST_CASE(non_value_token_constrctor_test, bdata::make(non_value_tokens_test_cases), type, position) {
    Token token{type, position};

    BOOST_CHECK_EQUAL(token.get_type(), type);
    BOOST_CHECK_EQUAL(token.get_position(), position);
    BOOST_CHECK_THROW(token.get_value_as<int>(), InvalidGetTokenValueException);
    BOOST_CHECK_THROW(token.get_value_as<double>(), InvalidGetTokenValueException);
    BOOST_CHECK_THROW(token.get_value_as<std::string>(), InvalidGetTokenValueException);
    BOOST_CHECK_THROW(token.get_value_as<bool>(), InvalidGetTokenValueException);
}

std::vector<std::tuple<TokenType, Position, int>> int_tokens_test_cases{
    {TokenType::T_LITERAL_INT, Position{1, 2}, 42},
    {TokenType::T_LITERAL_INT, Position{3, 4}, 17},
    {TokenType::T_LITERAL_INT, Position{5, 6}, 256},
    {TokenType::T_LITERAL_INT, Position{7, 8}, 1024},
    {TokenType::T_LITERAL_INT, Position{9, 10}, 73},
    {TokenType::T_LITERAL_INT, Position{11, 12}, 999},
    {TokenType::T_LITERAL_INT, Position{13, 14}, 12345},
    {TokenType::T_LITERAL_INT, Position{15, 16}, 54321},
    {TokenType::T_LITERAL_INT, Position{17, 18}, 0},
    {TokenType::T_LITERAL_INT, Position{19, 20}, 987},
};

BOOST_DATA_TEST_CASE(int_token_test, bdata::make(int_tokens_test_cases), type, position, value) {
    Token token{type, position, value};

    BOOST_CHECK_EQUAL(token.get_type(), type);
    BOOST_CHECK_EQUAL(token.get_position(), position);
    BOOST_CHECK_EQUAL(token.get_value_as<int>(), value);
    BOOST_CHECK_THROW(token.get_value_as<double>(), InvalidGetTokenValueException);
    BOOST_CHECK_THROW(token.get_value_as<std::string>(), InvalidGetTokenValueException);
    BOOST_CHECK_THROW(token.get_value_as<bool>(), InvalidGetTokenValueException);
}

std::vector<std::tuple<TokenType, Position, double>> float_tokens_test_cases{
    {TokenType::T_LITERAL_FLOAT, Position{1, 2}, 3.14159},
    {TokenType::T_LITERAL_FLOAT, Position{3, 4}, 2.71828},
    {TokenType::T_LITERAL_FLOAT, Position{5, 6}, 0.0},
    {TokenType::T_LITERAL_FLOAT, Position{7, 8}, 1.61803},
    {TokenType::T_LITERAL_FLOAT, Position{9, 10}, 23.456},
    {TokenType::T_LITERAL_FLOAT, Position{11, 12}, 9876.54321},
    {TokenType::T_LITERAL_FLOAT, Position{13, 14}, 0.0001},
    {TokenType::T_LITERAL_FLOAT, Position{15, 16}, 42.42},
    {TokenType::T_LITERAL_FLOAT, Position{17, 18}, 999.99},
    {TokenType::T_LITERAL_FLOAT, Position{19, 20}, 12345.6789},
};

BOOST_DATA_TEST_CASE(float_token_test, bdata::make(float_tokens_test_cases), type, position, value) {
    Token token{type, position, value};

    BOOST_CHECK_EQUAL(token.get_type(), type);
    BOOST_CHECK_EQUAL(token.get_position(), position);
    BOOST_CHECK_CLOSE(token.get_value_as<double>(), value, 0.001);
    BOOST_CHECK_THROW(token.get_value_as<int>(), InvalidGetTokenValueException);
    BOOST_CHECK_THROW(token.get_value_as<std::string>(), InvalidGetTokenValueException);
    BOOST_CHECK_THROW(token.get_value_as<bool>(), InvalidGetTokenValueException);
}

std::vector<std::tuple<TokenType, Position, std::string>> string_value_tokens_test_cases{
    {TokenType::T_LITERAL_STRING, Position{1, 2}, "Hello, World!"},
    {TokenType::T_LITERAL_STRING, Position{3, 4}, "^dhfjdffkjaabbd dfndsab sf"},
    {TokenType::T_LITERAL_STRING, Position{5, 6}, ""},
    {TokenType::T_LITERAL_STRING, Position{7, 8}, "Another example"},
    {TokenType::T_LITERAL_STRING, Position{9, 10}, "1234567890"},
    {TokenType::T_LITERAL_STRING, Position{11, 12}, "!@#$%^&*()"},
    {TokenType::T_LITERAL_STRING, Position{13, 14}, "Multiline\nString"},
    {TokenType::T_LITERAL_STRING, Position{15, 16}, "whites\t\n\r"},
    {TokenType::T_LITERAL_STRING, Position{17, 18}, "Foo Foo Foo"},
    {TokenType::T_LITERAL_STRING, Position{19, 20}, "fjdhsjghafjlkld''??"},
    {TokenType::T_IDENTIFIER, Position{1, 2}, "identifier1"},
    {TokenType::T_IDENTIFIER, Position{3, 4}, "_private_ident"},
    {TokenType::T_IDENTIFIER, Position{5, 6}, "camelCase"},
    {TokenType::T_IDENTIFIER, Position{7, 8}, "PascalCase"},
    {TokenType::T_IDENTIFIER, Position{9, 10}, "CONSTANT"},
    {TokenType::T_IDENTIFIER, Position{11, 12}, "x"},
    {TokenType::T_IDENTIFIER, Position{13, 14}, "is_correct"},
    {TokenType::T_IDENTIFIER, Position{15, 16}, "___"},
    {TokenType::T_IDENTIFIER, Position{17, 18}, "fjdkfjd"},
    {TokenType::T_IDENTIFIER, Position{19, 20}, "other123"},
    {TokenType::T_COMMENT, Position{1, 2}, "some commentd"},
    {TokenType::T_COMMENT, Position{3, 4}, "her eandofjdjrjhd"},
    {TokenType::T_COMMENT, Position{5, 6}, "some comment here !@#%$(#*&#$%)"},
    {TokenType::T_COMMENT, Position{7, 8}, "{}:FKDLFDHJHDJS:SDFHSJ"},
    {TokenType::T_COMMENT, Position{9, 10}, "another comment"},
    {TokenType::T_COMMENT, Position{11, 12}, "TODO: finish lexer"},
    {TokenType::T_COMMENT, Position{13, 14}, "TOO: add exception handling mechanism"},
    {TokenType::T_COMMENT, Position{15, 16}, "fdhfjdhjfhdjfhj"},
};

BOOST_DATA_TEST_CASE(string_value_token_test, bdata::make(string_value_tokens_test_cases), type, position, value) {
    Token token{type, position, value};

    BOOST_CHECK_EQUAL(token.get_type(), type);
    BOOST_CHECK_EQUAL(token.get_position(), position);
    BOOST_CHECK_EQUAL(token.get_value_as<std::string>(), value);
    BOOST_CHECK_THROW(token.get_value_as<int>(), InvalidGetTokenValueException);
    BOOST_CHECK_THROW(token.get_value_as<double>(), InvalidGetTokenValueException);
    BOOST_CHECK_THROW(token.get_value_as<bool>(), InvalidGetTokenValueException);
}

std::vector<std::tuple<Position, bool>> bool_values_test_cases{
    {Position{4, 34}, true},
    {Position{78, 123}, false},
};
BOOST_DATA_TEST_CASE(bool_values_test, bdata::make(bool_values_test_cases), position, value) {
    Token token{TokenType::T_LITERAL_BOOL, position, value};

    BOOST_CHECK_EQUAL(token.get_type()._to_integral(), TokenType::T_LITERAL_BOOL);
    BOOST_CHECK_EQUAL(token.get_position(), position);
    BOOST_CHECK_EQUAL(token.get_value_as<bool>(), value);
    BOOST_CHECK_THROW(token.get_value_as<int>(), InvalidGetTokenValueException);
    BOOST_CHECK_THROW(token.get_value_as<double>(), InvalidGetTokenValueException);
    BOOST_CHECK_THROW(token.get_value_as<std::string>(), InvalidGetTokenValueException);
}

std::vector<std::tuple<TokenType, Position>> value_tokens_without_values_test_cases{
    {TokenType::T_LITERAL_INT, Position{1, 1}},
    {TokenType::T_LITERAL_INT, Position{2, 3}},
    {TokenType::T_LITERAL_INT, Position{4, 5}},
    {TokenType::T_LITERAL_FLOAT, Position{6, 7}},
    {TokenType::T_LITERAL_FLOAT, Position{8, 9}},
    {TokenType::T_LITERAL_FLOAT, Position{10, 11}},
    {TokenType::T_LITERAL_STRING, Position{12, 13}},
    {TokenType::T_LITERAL_STRING, Position{14, 15}},
    {TokenType::T_LITERAL_STRING, Position{16, 17}},
    {TokenType::T_IDENTIFIER, Position{18, 19}},
    {TokenType::T_IDENTIFIER, Position{20, 21}},
    {TokenType::T_IDENTIFIER, Position{22, 23}},
};

BOOST_DATA_TEST_CASE(value_tokens_without_value_constructor_test, bdata::make(value_tokens_without_values_test_cases), type, position) {
    BOOST_CHECK_THROW(Token token(type, position), InvalidTokenValueException);
};

BOOST_DATA_TEST_CASE(non_value_tokens_constructor_recieving_value, bdata::make(non_value_tokens_test_cases), type, position) {
    int random_num = rand() % 3;

    std::variant<std::monostate, int, double, bool, std::string> value;

    switch (rand() % 5) {
        case 0:
            value = 42;
            break;
        case 1:
            value = 3.11434343;
            break;
        case 2:
            value = false;
        case 3:
            value = true;
        default:
            value = "some_string";
            break;
    }
    BOOST_CHECK_THROW(Token token(type, position, value), InvalidTokenValueException);
};

std::vector<std::tuple<Token, std::string>> token_print_test_case{
    {Token{TokenType::T_AND, Position{4, 4}}, "Token(T_AND,Position(4,4))"},
    {Token{TokenType::T_INT, Position{9, 41}}, "Token(T_INT,Position(9,41))"},
    {Token{TokenType::T_FLOAT, Position{34, 3}}, "Token(T_FLOAT,Position(34,3))"},
    {Token{TokenType::T_FUNCTION, Position{90, 65}}, "Token(T_FUNCTION,Position(90,65))"},
    {Token{TokenType::T_NONE, Position{77, 7}}, "Token(T_NONE,Position(77,7))"},
    {Token{TokenType::T_FUNC_COMPOSITION, Position{84, 1}}, "Token(T_FUNC_COMPOSITION,Position(84,1))"},
    {Token{TokenType::T_ASSIGN, Position{92, 11}}, "Token(T_ASSIGN,Position(92,11))"},
    {Token{TokenType::T_L_BRACE, Position{7, 14}}, "Token(T_L_BRACE,Position(7,14))"},
    {Token{TokenType::T_EOF, Position{1004, 1}}, "Token(T_EOF,Position(1004,1))"},
    {Token{TokenType::T_COMMA, Position{62, 8}}, "Token(T_COMMA,Position(62,8))"},
    {Token{TokenType::T_LITERAL_INT, Position{34, 3}, 123456789}, "Token(T_LITERAL_INT,Position(34,3),123456789)"},
    {Token{TokenType::T_LITERAL_FLOAT, Position{90, 65}, 3.1422222}, "Token(T_LITERAL_FLOAT,Position(90,65),3.142222)"},
    {Token{TokenType::T_LITERAL_STRING, Position{77, 7}, "Hello world!"}, "Token(T_LITERAL_STRING,Position(77,7),\"Hello world!\")"},
    {Token{TokenType::T_IDENTIFIER, Position{84, 1}, "_variable1"}, "Token(T_IDENTIFIER,Position(84,1),\"_variable1\")"},
};

BOOST_DATA_TEST_CASE(token_print_test, bdata::make(token_print_test_case), token, expected_output) {
    BOOST_CHECK_EQUAL(token.print(), expected_output);
}