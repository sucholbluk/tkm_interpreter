#define BOOST_TEST_MODULE INTERPRETER_TESTS
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/included/unit_test.hpp>

#include "interpreter.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "source_handler.hpp"

std::unique_ptr<Program> get_program(std::string mock_file) {
    std::unique_ptr<std::istream> source = std::make_unique<std::stringstream>(mock_file);
    auto source_handler = std::make_unique<SourceHandler>(std::move(source));
    std::unique_ptr<ILexer> _lexer = std::make_unique<Lexer>(std::move(source_handler));
    Parser parser{std::move(_lexer)};
    return parser.parse_program();
}

BOOST_AUTO_TEST_CASE(fib_test) {
    std::string expected_output{"For 7 sequence number is 13\n"};
    std::string mock_file = R"(
def nth_fibonacci(n: int) -> int { #some comment here
    if (n <= 1) {
        return n;
    }

    return nth_fibonacci(n - 1) + nth_fibonacci(n - 2);
}
def main() -> int {
    let n: int = 7;
    print("For " + n as string + " sequence number is " + nth_fibonacci(n) as string);
}
)";
    Interpreter interpreter{};
    auto program{get_program(mock_file)};
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    program->accept(interpreter);

    std::string output = buffer.str();
    std::cout.rdbuf(old);

    BOOST_CHECK(output == expected_output);
}

BOOST_AUTO_TEST_CASE(print_literal_string_test) {
    std::string expected_output{"Hello world!\n"};
    std::string mock_file = R"(
def main() -> int {
    print("Hello world!");
    return 0;
}
)";
    Interpreter interpreter{};
    auto program{get_program(mock_file)};
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    program->accept(interpreter);

    std::string output = buffer.str();
    std::cout.rdbuf(old);

    BOOST_CHECK(output == expected_output);
}

BOOST_AUTO_TEST_CASE(to_string_type_cast_test) {
    std::string expected_output{"true\n190\n3.12345\n3\n"};
    std::string mock_file = R"(
def main() -> int {
    print(true as string);
    print(190 as string);
    print(3.12345 as string);
    print(3.0 as string); # if no fraction, formatted like ints
    return 0;
}
)";
    Interpreter interpreter{};
    auto program{get_program(mock_file)};
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    program->accept(interpreter);

    std::string output = buffer.str();
    std::cout.rdbuf(old);

    BOOST_CHECK(output == expected_output);
}

BOOST_AUTO_TEST_CASE(to_int_type_cast_test) {
    std::string expected_output{"12345\n3\n1\n0\n"};
    std::string mock_file = R"(
def print_int(a: int) -> none {
    print(a as string);
}

def main() -> int {
    print_int("12345" as int);
    print_int(3.9 as int);
    print_int(true as int);
    print_int(false as int);
    
    return 0;
}

)";
    Interpreter interpreter{};
    auto program{get_program(mock_file)};
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    program->accept(interpreter);

    std::string output = buffer.str();
    std::cout.rdbuf(old);

    BOOST_CHECK(output == expected_output);
}

BOOST_AUTO_TEST_CASE(to_bool_type_cast_test) {
    std::string expected_output{"false\ntrue\nfalse\ntrue\nfalse\ntrue\n"};
    std::string mock_file = R"(
def print_bool(a: bool) -> none {
    print(a as string);
}

def main() -> int {
    print_bool("" as bool); # false
    print_bool("a" as bool); # true

    print_bool(0 as bool); # false
    print_bool( 23 as bool); # true

    print_bool(0.0 as bool); # false
    print_bool(0.12 as bool); # true
    return 0;
}

)";
    Interpreter interpreter{};
    auto program{get_program(mock_file)};
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    program->accept(interpreter);

    std::string output = buffer.str();
    std::cout.rdbuf(old);

    BOOST_CHECK(output == expected_output);
}

BOOST_AUTO_TEST_CASE(to_float_type_cast_test) {
    std::string expected_output{"12345\n3\n1\n0\n12.5\n"};
    std::string mock_file = R"(
def print_float(a: float) -> none {
    print(a as string);
}

def main() -> int {
    print_float("12345" as float);
    print_float(3 as float);
    print_float(true as float);
    print_float(false as float);
    print_float("12.5" as float);
    return 0;
}
)";
    Interpreter interpreter{};
    auto program{get_program(mock_file)};
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    program->accept(interpreter);

    std::string output = buffer.str();
    std::cout.rdbuf(old);

    BOOST_CHECK(output == expected_output);
}

BOOST_AUTO_TEST_CASE(variable_simple_test) {
    std::string expected_output{"123\n3.14\nfalse\n"};
    std::string mock_file = R"(
def main() -> int {
    let i: int = 123;
    let mut pi: float = 3.14;
    let always_false: bool = false;
    print(i as string);
    print(pi as string);
    print(always_false as string);
    return 0;
}
)";
    Interpreter interpreter{};
    auto program{get_program(mock_file)};
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    program->accept(interpreter);

    std::string output = buffer.str();
    std::cout.rdbuf(old);

    BOOST_CHECK(output == expected_output);
}

BOOST_AUTO_TEST_CASE(immutable_func_param_test) {
    std::string expected_output{"0\n8\n5\n4\n"};
    std::string mock_file = R"(
def print_int(a: int) -> none {
    print(a as string);
}

def main() -> int {
    let num: int = 0;
    let mut mut_num: int = 5;

    print_int(num);
    print_int(8);
    print_int(mut_num);
    
    mut_num = mut_num - 1;
    print_int(mut_num);

    return 0;
}
)";
    Interpreter interpreter{};
    auto program{get_program(mock_file)};
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    program->accept(interpreter);

    std::string output = buffer.str();
    std::cout.rdbuf(old);

    BOOST_CHECK(output == expected_output);
}

BOOST_AUTO_TEST_CASE(mutable_param_test) {
    std::string expected_output{"1\n6\n2\n3\n4\n"};
    std::string mock_file = R"(
def inc_and_print(mut a: int) -> none {
    a = a + 1;
    print(a as string);
}

def main() -> int {
    let mut mut_num: int = 0;

    inc_and_print(mut_num);

    # mozemy podac rval na wejscie, wtedy po prostu wewnatrz funkcji tworzona jest mutowalna zmienna lokalna
    # ze wzgledu na to, ze zmienne przyjmujemy przez referencje, niemutowalnej zmiennej tak nie mozemy podac 
    inc_and_print(5);

    inc_and_print(mut_num);
    inc_and_print(mut_num);
    inc_and_print(mut_num);

    return 0;
}
)";
    Interpreter interpreter{};
    auto program{get_program(mock_file)};
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    program->accept(interpreter);

    std::string output = buffer.str();
    std::cout.rdbuf(old);

    BOOST_CHECK(output == expected_output);
}

BOOST_AUTO_TEST_CASE(if_condition_true_test) {
    std::string expected_output{"condition met\n"};
    std::string mock_file = R"(
def main() -> int {
    if (true) {
        print("condition met");
    }
    return 0;
}
)";
    Interpreter interpreter{};
    auto program{get_program(mock_file)};
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    program->accept(interpreter);

    std::string output = buffer.str();
    std::cout.rdbuf(old);

    BOOST_CHECK(output == expected_output);
}

BOOST_AUTO_TEST_CASE(if_condition_false_test) {
    std::string expected_output{""};
    std::string mock_file = R"(
def main() -> int {
    if (false) {
        print("there should be no output");
    }
    return 0;
}
)";
    Interpreter interpreter{};
    auto program{get_program(mock_file)};
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    program->accept(interpreter);

    std::string output = buffer.str();
    std::cout.rdbuf(old);

    BOOST_CHECK(output == expected_output);
}

BOOST_AUTO_TEST_CASE(if_else_condition_test) {
    std::string expected_output{"else path\n"};
    std::string mock_file = R"(
def main() -> int {
    if (false) {
        print("if path");
    } else {
        print("else path");
    }
    return 0;
}
)";
    Interpreter interpreter{};
    auto program{get_program(mock_file)};
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    program->accept(interpreter);

    std::string output = buffer.str();
    std::cout.rdbuf(old);

    BOOST_CHECK(output == expected_output);
}

BOOST_AUTO_TEST_CASE(if_elseif_else_condition_test) {
    std::string expected_output{"smaller\ngreater\nequal\n"};
    std::string mock_file = R"(
def compare(a: int, b: int) -> none {
    if (a < b) {
        print("smaller");
    } else if (a > b) {
        print("greater");
    } else {
        print("equal");
    }
}

def main() -> int {
    compare(1, 2);
    compare(8, 7);
    compare(120, 120);
    return 0;
}
)";
    Interpreter interpreter{};
    auto program{get_program(mock_file)};
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    program->accept(interpreter);

    std::string output = buffer.str();
    std::cout.rdbuf(old);

    BOOST_CHECK(output == expected_output);
}
