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
    Token plus{lexer.get_next_token()};
    Token eof{lexer.get_next_token()};
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
    {"# here is a comment", TokenType::T_COMMENT},
    {"\"Here is the string$#@!@!er3343\"", TokenType::T_LITERAL_STRING},
    {"\"string with space after that should not be tokenized\"   ", TokenType::T_LITERAL_STRING}

};

BOOST_DATA_TEST_CASE(simple_tests, bdata::make(simple_test_cases), input, type) {
    std::stringstream source{input};
    auto handler = std::make_unique<SourceHandler>(source);

    Lexer lexer{std::move(handler)};
    Token tk{lexer.get_next_token()};
    Token eof{lexer.get_next_token()};

    BOOST_CHECK_EQUAL(tk.get_type(), type);
    BOOST_CHECK_EQUAL(tk.get_position(), Position{});
    BOOST_CHECK(eof.get_type()._to_integral() == TokenType::T_EOF);
}
std::vector<std::tuple<std::string, int>> int_test_cases{
    {"0", 0},
    {"123", 123},
    {"456789", 456789},
    {"2147483647", 2147483647},
};

BOOST_DATA_TEST_CASE(int_tests, bdata::make(int_test_cases), input, expected_value) {
    std::stringstream source{input};
    auto handler = std::make_unique<SourceHandler>(source);

    Lexer lexer{std::move(handler)};
    Token tk = lexer.get_next_token();

    BOOST_CHECK_EQUAL(tk.get_type()._to_integral(), TokenType::T_LITERAL_INT);
    BOOST_CHECK_EQUAL(tk.get_value_as<int>(), expected_value);

    Token eof = lexer.get_next_token();
    BOOST_CHECK_EQUAL(eof.get_type()._to_integral(), TokenType::T_EOF);
}

std::vector<std::tuple<std::string, double>> float_test_cases{
    {"0.0", 0.0},
    {"12312.456222222", 12312.456222222},
    {"78912.0191233232", 78912.0191233232},
    {"31212.14159", 31212.14159},
    {"120222.500", 120222.500},
};

BOOST_DATA_TEST_CASE(float_tests, bdata::make(float_test_cases), input, expected_value) {
    std::stringstream source{input};
    auto handler = std::make_unique<SourceHandler>(source);

    Lexer lexer{std::move(handler)};
    Token tk = lexer.get_next_token();

    BOOST_CHECK_EQUAL(tk.get_type()._to_integral(), TokenType::T_LITERAL_FLOAT);
    BOOST_CHECK_CLOSE(tk.get_value_as<double>(), expected_value, 0.01);

    Token eof = lexer.get_next_token();
    BOOST_CHECK_EQUAL(eof.get_type()._to_integral(), TokenType::T_EOF);
}

BOOST_AUTO_TEST_CASE(fibbonacci_test) {
    std::stringstream mock_file;
    mock_file << R"EOF(
def nth_fibonacci(n: int) -> int {
    if (n <= 1) {
        return n;
    }

    return nth_fibonacci(n - 1) + nth_fibonacci(n - 2);
}

let n: int = 5;

print("For " + n as string + " sequence number is " + nth_fibonacci(n) as string);
)EOF";
    auto handler = std::make_unique<SourceHandler>(mock_file);
    Lexer lexer{std::move(handler)};

    Token def{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(def.get_type()._to_integral(), TokenType::T_DEF);
    BOOST_CHECK_EQUAL(def.get_position(), Position(2, 1));

    Token nth_fibonacci{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(nth_fibonacci.get_type()._to_integral(), TokenType::T_IDENTIFIER);
    BOOST_CHECK_EQUAL(nth_fibonacci.get_position(), Position(2, 5));
    BOOST_CHECK_EQUAL(nth_fibonacci.get_value_as<std::string>(), std::string("nth_fibonacci"));

    Token l_paren{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(l_paren.get_type()._to_integral(), TokenType::T_L_PAREN);
    BOOST_CHECK_EQUAL(l_paren.get_position(), Position(2, 18));

    Token n{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(n.get_type()._to_integral(), TokenType::T_IDENTIFIER);
    BOOST_CHECK_EQUAL(n.get_position(), Position(2, 19));
    BOOST_CHECK_EQUAL(n.get_value_as<std::string>(), std::string("n"));

    Token colon{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(colon.get_type()._to_integral(), TokenType::T_COLON);
    BOOST_CHECK_EQUAL(colon.get_position(), Position(2, 20));

    Token int_type{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(int_type.get_type()._to_integral(), TokenType::T_INT);
    BOOST_CHECK_EQUAL(int_type.get_position(), Position(2, 22));

    Token r_paren{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(r_paren.get_type()._to_integral(), TokenType::T_R_PAREN);
    BOOST_CHECK_EQUAL(r_paren.get_position(), Position(2, 25));

    Token arrow{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(arrow.get_type()._to_integral(), TokenType::T_ARROW);
    BOOST_CHECK_EQUAL(arrow.get_position(), Position(2, 27));

    Token int_return{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(int_return.get_type()._to_integral(), TokenType::T_INT);
    BOOST_CHECK_EQUAL(int_return.get_position(), Position(2, 30));

    Token l_brace{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(l_brace.get_type()._to_integral(), TokenType::T_L_BRACE);
    BOOST_CHECK_EQUAL(l_brace.get_position(), Position(2, 34));

    Token if_token{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(if_token.get_type()._to_integral(), TokenType::T_IF);
    BOOST_CHECK_EQUAL(if_token.get_position(), Position(3, 5));

    Token l_paren_if{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(l_paren_if.get_type()._to_integral(), TokenType::T_L_PAREN);
    BOOST_CHECK_EQUAL(l_paren_if.get_position(), Position(3, 8));

    Token n_if{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(n_if.get_type()._to_integral(), TokenType::T_IDENTIFIER);
    BOOST_CHECK_EQUAL(n_if.get_position(), Position(3, 9));
    BOOST_CHECK_EQUAL(n.get_value_as<std::string>(), std::string("n"));

    Token less_equal{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(less_equal.get_type()._to_integral(), TokenType::T_LESS_EQUAL);
    BOOST_CHECK_EQUAL(less_equal.get_position(), Position(3, 11));

    Token one{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(one.get_type()._to_integral(), TokenType::T_LITERAL_INT);
    BOOST_CHECK_EQUAL(one.get_position(), Position(3, 14));
    BOOST_CHECK_EQUAL(one.get_value_as<int>(), 1);

    Token r_paren_if{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(r_paren_if.get_type()._to_integral(), TokenType::T_R_PAREN);
    BOOST_CHECK_EQUAL(r_paren_if.get_position(), Position(3, 15));

    Token l_brace_if{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(l_brace_if.get_type()._to_integral(), TokenType::T_L_BRACE);
    BOOST_CHECK_EQUAL(l_brace_if.get_position(), Position(3, 17));

    Token return_token{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(return_token.get_type()._to_integral(), TokenType::T_RETURN);
    BOOST_CHECK_EQUAL(return_token.get_position(), Position(4, 9));

    Token n_return{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(n_return.get_type()._to_integral(), TokenType::T_IDENTIFIER);
    BOOST_CHECK_EQUAL(n_return.get_position(), Position(4, 16));
    BOOST_CHECK_EQUAL(n.get_value_as<std::string>(), std::string("n"));

    Token semicolon{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(semicolon.get_type()._to_integral(), TokenType::T_SEMICOLON);
    BOOST_CHECK_EQUAL(semicolon.get_position(), Position(4, 17));

    Token r_brace{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(r_brace.get_type()._to_integral(), TokenType::T_R_BRACE);
    BOOST_CHECK_EQUAL(r_brace.get_position(), Position(5, 5));

    Token return_2nd{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(return_2nd.get_type()._to_integral(), TokenType::T_RETURN);
    BOOST_CHECK_EQUAL(return_2nd.get_position(), Position(7, 5));

    nth_fibonacci = lexer.get_next_token();
    BOOST_CHECK_EQUAL(nth_fibonacci.get_type()._to_integral(), TokenType::T_IDENTIFIER);
    BOOST_CHECK_EQUAL(nth_fibonacci.get_position(), Position(7, 12));
    BOOST_CHECK_EQUAL(nth_fibonacci.get_value_as<std::string>(), std::string("nth_fibonacci"));

    l_paren = lexer.get_next_token();
    BOOST_CHECK_EQUAL(l_paren.get_type()._to_integral(), TokenType::T_L_PAREN);
    BOOST_CHECK_EQUAL(l_paren.get_position(), Position(7, 25));

    n = lexer.get_next_token();
    BOOST_CHECK_EQUAL(n.get_type()._to_integral(), TokenType::T_IDENTIFIER);
    BOOST_CHECK_EQUAL(n.get_position(), Position(7, 26));
    BOOST_CHECK_EQUAL(n.get_value_as<std::string>(), std::string("n"));

    Token minus{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(minus.get_type()._to_integral(), TokenType::T_MINUS);
    BOOST_CHECK_EQUAL(minus.get_position(), Position(7, 28));

    one = lexer.get_next_token();
    BOOST_CHECK_EQUAL(one.get_type()._to_integral(), TokenType::T_LITERAL_INT);
    BOOST_CHECK_EQUAL(one.get_position(), Position(7, 30));
    BOOST_CHECK_EQUAL(one.get_value_as<int>(), 1);

    r_paren = lexer.get_next_token();
    BOOST_CHECK_EQUAL(r_paren.get_type()._to_integral(), TokenType::T_R_PAREN);
    BOOST_CHECK_EQUAL(r_paren.get_position(), Position(7, 31));

    Token plus{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(plus.get_type()._to_integral(), TokenType::T_PLUS);
    BOOST_CHECK_EQUAL(plus.get_position(), Position(7, 33));

    nth_fibonacci = lexer.get_next_token();
    BOOST_CHECK_EQUAL(nth_fibonacci.get_type()._to_integral(), TokenType::T_IDENTIFIER);
    BOOST_CHECK_EQUAL(nth_fibonacci.get_position(), Position(7, 35));
    BOOST_CHECK_EQUAL(nth_fibonacci.get_value_as<std::string>(), std::string("nth_fibonacci"));

    l_paren = lexer.get_next_token();
    BOOST_CHECK_EQUAL(l_paren.get_type()._to_integral(), TokenType::T_L_PAREN);
    BOOST_CHECK_EQUAL(l_paren.get_position(), Position(7, 48));

    n = lexer.get_next_token();
    BOOST_CHECK_EQUAL(n.get_type()._to_integral(), TokenType::T_IDENTIFIER);
    BOOST_CHECK_EQUAL(n.get_position(), Position(7, 49));
    BOOST_CHECK_EQUAL(n.get_value_as<std::string>(), std::string("n"));

    minus = lexer.get_next_token();
    BOOST_CHECK_EQUAL(minus.get_type()._to_integral(), TokenType::T_MINUS);
    BOOST_CHECK_EQUAL(minus.get_position(), Position(7, 51));

    Token two{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(two.get_type()._to_integral(), TokenType::T_LITERAL_INT);
    BOOST_CHECK_EQUAL(two.get_position(), Position(7, 53));
    BOOST_CHECK_EQUAL(two.get_value_as<int>(), 2);

    r_paren = lexer.get_next_token();
    BOOST_CHECK_EQUAL(r_paren.get_type()._to_integral(), TokenType::T_R_PAREN);
    BOOST_CHECK_EQUAL(r_paren.get_position(), Position(7, 54));

    semicolon = lexer.get_next_token();
    BOOST_CHECK_EQUAL(semicolon.get_type()._to_integral(), TokenType::T_SEMICOLON);
    BOOST_CHECK_EQUAL(semicolon.get_position(), Position(7, 55));

    r_brace = lexer.get_next_token();
    BOOST_CHECK_EQUAL(r_brace.get_type()._to_integral(), TokenType::T_R_BRACE);
    BOOST_CHECK_EQUAL(r_brace.get_position(), Position(8, 1));

    Token let{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(let.get_type()._to_integral(), TokenType::T_LET);
    BOOST_CHECK_EQUAL(let.get_position(), Position(10, 1));

    n = lexer.get_next_token();
    BOOST_CHECK_EQUAL(n.get_type()._to_integral(), TokenType::T_IDENTIFIER);
    BOOST_CHECK_EQUAL(n.get_position(), Position(10, 5));
    BOOST_CHECK_EQUAL(n.get_value_as<std::string>(), std::string("n"));

    colon = lexer.get_next_token();
    BOOST_CHECK_EQUAL(colon.get_type()._to_integral(), TokenType::T_COLON);
    BOOST_CHECK_EQUAL(colon.get_position(), Position(10, 6));

    int_type = lexer.get_next_token();
    BOOST_CHECK_EQUAL(int_type.get_type()._to_integral(), TokenType::T_INT);
    BOOST_CHECK_EQUAL(int_type.get_position(), Position(10, 8));

    Token assign{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(assign.get_type()._to_integral(), TokenType::T_ASSIGN);
    BOOST_CHECK_EQUAL(assign.get_position(), Position(10, 12));

    Token five{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(five.get_type()._to_integral(), TokenType::T_LITERAL_INT);
    BOOST_CHECK_EQUAL(five.get_position(), Position(10, 14));
    BOOST_CHECK_EQUAL(five.get_value_as<int>(), 5);

    semicolon = lexer.get_next_token();
    BOOST_CHECK_EQUAL(semicolon.get_type()._to_integral(), TokenType::T_SEMICOLON);
    BOOST_CHECK_EQUAL(semicolon.get_position(), Position(10, 15));

    Token print{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(print.get_type()._to_integral(), TokenType::T_IDENTIFIER);
    BOOST_CHECK_EQUAL(print.get_position(), Position(12, 1));
    BOOST_CHECK_EQUAL(print.get_value_as<std::string>(), std::string("print"));

    l_paren = lexer.get_next_token();
    BOOST_CHECK_EQUAL(l_paren.get_type()._to_integral(), TokenType::T_L_PAREN);
    BOOST_CHECK_EQUAL(l_paren.get_position(), Position(12, 6));

    Token for_str{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(for_str.get_type()._to_integral(), TokenType::T_LITERAL_STRING);
    BOOST_CHECK_EQUAL(for_str.get_position(), Position(12, 7));
    BOOST_CHECK_EQUAL(for_str.get_value_as<std::string>(), std::string("For "));

    plus = lexer.get_next_token();
    BOOST_CHECK_EQUAL(plus.get_type()._to_integral(), TokenType::T_PLUS);
    BOOST_CHECK_EQUAL(plus.get_position(), Position(12, 14));

    n = lexer.get_next_token();
    BOOST_CHECK_EQUAL(n.get_type()._to_integral(), TokenType::T_IDENTIFIER);
    BOOST_CHECK_EQUAL(n.get_position(), Position(12, 16));
    BOOST_CHECK_EQUAL(n.get_value_as<std::string>(), std::string("n"));

    Token as{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(as.get_type()._to_integral(), TokenType::T_AS);
    BOOST_CHECK_EQUAL(as.get_position(), Position(12, 18));

    Token string_type{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(string_type.get_type()._to_integral(), TokenType::T_STRING);
    BOOST_CHECK_EQUAL(string_type.get_position(), Position(12, 21));

    plus = lexer.get_next_token();
    BOOST_CHECK_EQUAL(plus.get_type()._to_integral(), TokenType::T_PLUS);
    BOOST_CHECK_EQUAL(plus.get_position(), Position(12, 28));

    Token seq_string{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(seq_string.get_type()._to_integral(), TokenType::T_LITERAL_STRING);
    BOOST_CHECK_EQUAL(seq_string.get_position(), Position(12, 30));
    BOOST_CHECK_EQUAL(seq_string.get_value_as<std::string>(), std::string(" sequence number is "));

    plus = lexer.get_next_token();
    BOOST_CHECK_EQUAL(plus.get_type()._to_integral(), TokenType::T_PLUS);
    BOOST_CHECK_EQUAL(plus.get_position(), Position(12, 53));

    nth_fibonacci = lexer.get_next_token();
    BOOST_CHECK_EQUAL(nth_fibonacci.get_type()._to_integral(), TokenType::T_IDENTIFIER);
    BOOST_CHECK_EQUAL(nth_fibonacci.get_position(), Position(12, 55));
    BOOST_CHECK_EQUAL(nth_fibonacci.get_value_as<std::string>(), std::string("nth_fibonacci"));

    l_paren = lexer.get_next_token();
    BOOST_CHECK_EQUAL(l_paren.get_type()._to_integral(), TokenType::T_L_PAREN);
    BOOST_CHECK_EQUAL(l_paren.get_position(), Position(12, 68));

    n = lexer.get_next_token();
    BOOST_CHECK_EQUAL(n.get_type()._to_integral(), TokenType::T_IDENTIFIER);
    BOOST_CHECK_EQUAL(n.get_position(), Position(12, 69));
    BOOST_CHECK_EQUAL(n.get_value_as<std::string>(), std::string("n"));

    r_paren = lexer.get_next_token();
    BOOST_CHECK_EQUAL(r_paren.get_type()._to_integral(), TokenType::T_R_PAREN);
    BOOST_CHECK_EQUAL(r_paren.get_position(), Position(12, 70));

    as = lexer.get_next_token();
    BOOST_CHECK_EQUAL(as.get_type()._to_integral(), TokenType::T_AS);
    BOOST_CHECK_EQUAL(as.get_position(), Position(12, 72));

    string_type = lexer.get_next_token();
    BOOST_CHECK_EQUAL(string_type.get_type()._to_integral(), TokenType::T_STRING);
    BOOST_CHECK_EQUAL(string_type.get_position(), Position(12, 75));

    r_paren = lexer.get_next_token();
    BOOST_CHECK_EQUAL(r_paren.get_type()._to_integral(), TokenType::T_R_PAREN);
    BOOST_CHECK_EQUAL(r_paren.get_position(), Position(12, 81));

    semicolon = lexer.get_next_token();
    BOOST_CHECK_EQUAL(semicolon.get_type()._to_integral(), TokenType::T_SEMICOLON);
    BOOST_CHECK_EQUAL(semicolon.get_position(), Position(12, 82));

    Token eof{lexer.get_next_token()};
    BOOST_CHECK_EQUAL(eof.get_type()._to_integral(), TokenType::T_EOF);
    BOOST_CHECK_EQUAL(eof.get_position(), Position(13, 1));

    eof = lexer.get_next_token();
    BOOST_CHECK_EQUAL(eof.get_type()._to_integral(), TokenType::T_EOF);
    BOOST_CHECK_EQUAL(eof.get_position(), Position(13, 1));
}
