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

BOOST_AUTO_TEST_CASE(addition_int_test) {
    std::string expected_output{"5\n0\n-7\n"};
    std::string mock_file = R"(
def main() -> int {
    print((2 + 3) as string);
    print((2 + -2) as string);
    print((-3 + -4) as string);
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

BOOST_AUTO_TEST_CASE(addition_float_test) {
    std::string expected_output{"5.5\n0\n-7.25\n"};
    std::string mock_file = R"(
def main() -> int {
    print((2.0 + 3.5) as string);
    print((2.5 + -2.5) as string);
    print((-3.0 + -4.25) as string);
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

BOOST_AUTO_TEST_CASE(addition_string_test) {
    std::string expected_output{"hello world\nfoo42\nabc123def\n"};
    std::string mock_file = R"(
def main() -> int {
    print("hello " + "world");
    print("foo" + 42 as string);
    print("abc" + 123 as string + "def");
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

BOOST_AUTO_TEST_CASE(subtraction_int_test) {
    std::string expected_output{"-1\n4\n1\n"};
    std::string mock_file = R"(
def main() -> int {
    print((2 - 3) as string);
    print((6 - 2) as string);
    print((5 - 4) as string);
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

BOOST_AUTO_TEST_CASE(subtraction_float_test) {
    std::string expected_output{"-1.5\n4\n0.75\n"};
    std::string mock_file = R"(
def main() -> int {
    print((2.0 - 3.5) as string);
    print((6.0 - 2.0) as string);
    print((5.25 - 4.5) as string);
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

BOOST_AUTO_TEST_CASE(multiplication_int_test) {
    std::string expected_output{"6\n0\n-12\n"};
    std::string mock_file = R"(
def main() -> int {
    print((2 * 3) as string);
    print((2 * 0) as string);
    print((-3 * 4) as string);
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

BOOST_AUTO_TEST_CASE(multiplication_float_test) {
    std::string expected_output{"7.5\n0\n-19.1625\n"};
    std::string mock_file = R"(
def main() -> int {
    print((2.5 * 3.0) as string);
    print((2.5 * 0.0) as string);
    print(round(-3.5 * 5.475, 4) as string);
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

    std::cout << output;
    BOOST_CHECK(output == expected_output);
}

BOOST_AUTO_TEST_CASE(division_int_test) {
    std::string expected_output{"2\n0\n-3\n"};
    std::string mock_file = R"(
def main() -> int {
    print((6 / 3) as string);
    print((2 / 3) as string);
    print((-9 / 3) as string);
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

BOOST_AUTO_TEST_CASE(division_float_test) {
    std::string expected_output{"2.5\n0.66667\n-1.75\n"};
    std::string mock_file = R"(
def main() -> int {
    print(round(5.0 / 2.0, 5) as string);
    print(round(2.0 / 3.0, 5) as string);
    print(round(-3.5 / 2.0, 5) as string);
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

BOOST_AUTO_TEST_CASE(complex_int_operations_test) {
    std::string expected_output{"8\n9\n-8\n5\n"};
    std::string mock_file = R"(
def main() -> int {
    print((2 + 3 * 4 / 2) as string);         # 2 + 6 = 8
    print((10 - 3 * 2 + 5) as string);        # 10 - 6 + 5 = 9
    print(-(8 / 2 + 3 * 2 - 2) as string);     # 4 + 6 - 2 = 8
    print((7 + 3 * 2 - 8 / 4 - 6) as string); # 7 + 6 - 2 - 6 = 5
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

BOOST_AUTO_TEST_CASE(complex_float_operations_test) {
    std::string expected_output{"9\n-8.75\n6.75\n4.5\n"};
    std::string mock_file = R"(
def main() -> int {
    print((2.5 + 3.0 * 4.0 - 5.5) as string);         # 2.5 + 12.0 - 5.5 = 9.0
    print(-(10.0 - 3.5 * 2.0 + 5.75) as string);       # 10.0 - 7.0 + 5.75 = 8.75
    print((8.0 / 4.0 + 3.5 * 2.0 - 2.25) as string);  # 2.0 + 7.0 - 2.25 = 6.75
    print((7.0 + 3.0 * 2.0 - 8.0 / 4.0 - 6.5) as string); # 7.0 + 6.0 - 2.0 - 6.5 = 4.5
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

BOOST_AUTO_TEST_CASE(equality_and_inequality_int_test) {
    std::string expected_output{"true\nfalse\nfalse\ntrue\n"};
    std::string mock_file = R"(
def main() -> int {
    print((5 == 5) as string);    # true
    print((5 == 3) as string);    # false
    print((5 != 5) as string);    # false
    print((5 != 3) as string);    # true
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

BOOST_AUTO_TEST_CASE(equality_and_inequality_float_test) {
    std::string expected_output{"true\nfalse\nfalse\ntrue\n"};
    std::string mock_file = R"(
def main() -> int {
    print((3.14 == 3.14) as string);    # true
    print((3.14 == 2.71) as string);    # false
    print((3.14 != 3.14) as string);    # false
    print((3.14 != 2.71) as string);    # true
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

BOOST_AUTO_TEST_CASE(equality_and_inequality_string_test) {
    std::string expected_output{"true\nfalse\nfalse\ntrue\n"};
    std::string mock_file = R"(
def main() -> int {
    print(("abc" == "abc") as string);    # true
    print(("abc" == "def") as string);    # false
    print(("abc" != "abc") as string);    # false
    print(("abc" != "def") as string);    # true
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

BOOST_AUTO_TEST_CASE(equality_and_inequality_bool_test) {
    std::string expected_output{"true\nfalse\nfalse\ntrue\n"};
    std::string mock_file = R"(
def main() -> int {
    print((true == true) as string);    # true
    print((true == false) as string);   # false
    print((true != true) as string);    # false
    print((true != false) as string);   # true
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

BOOST_AUTO_TEST_CASE(relational_int_test) {
    std::string expected_output{"false\ntrue\ntrue\nfalse\ntrue\ntrue\nfalse\nfalse\n"};
    std::string mock_file = R"(
def main() -> int {
    print((5 < 5) as string);     # false
    print((3 < 5) as string);     # true
    print((5 > 3) as string);     # true
    print((3 > 5) as string);     # false
    print((5 <= 5) as string);    # true
    print((3 <= 5) as string);    # true
    print((5 >= 7) as string);    # false
    print((2 >= 3) as string);    # false
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

BOOST_AUTO_TEST_CASE(relational_float_test) {
    std::string expected_output{"false\ntrue\ntrue\nfalse\ntrue\ntrue\nfalse\nfalse\n"};
    std::string mock_file = R"(
def main() -> int {
    print((3.14 < 3.14) as string);     # false
    print((2.71 < 3.14) as string);     # true
    print((3.14 > 2.71) as string);     # true
    print((2.71 > 3.14) as string);     # false
    print((3.14 <= 3.14) as string);    # true
    print((2.71 <= 3.14) as string);    # true
    print((3.14 >= 4.0) as string);     # false
    print((2.0 >= 3.0) as string);      # false
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

BOOST_AUTO_TEST_CASE(relational_string_test) {
    std::string expected_output{"false\ntrue\ntrue\nfalse\ntrue\ntrue\nfalse\nfalse\n"};
    std::string mock_file = R"(
def main() -> int {
    print(("abc" < "abc") as string);     # false
    print(("abc" < "def") as string);     # true
    print(("def" > "abc") as string);     # true
    print(("abc" > "def") as string);     # false
    print(("abc" <= "abc") as string);    # true
    print(("abc" <= "def") as string);    # true
    print(("def" >= "xyz") as string);    # false
    print(("abc" >= "def") as string);    # false
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

BOOST_AUTO_TEST_CASE(relational_bool_test) {
    std::string expected_output{"false\ntrue\ntrue\nfalse\ntrue\ntrue\ntrue\nfalse\n"};
    std::string mock_file = R"(
def main() -> int {
    print((true < true) as string);     # false
    print((false < true) as string);    # true
    print((true > false) as string);    # true
    print((false > true) as string);    # false
    print((true <= true) as string);    # true
    print((false <= true) as string);   # true
    print((true >= false) as string);   # true
    print((false >= true) as string);   # false
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

BOOST_AUTO_TEST_CASE(logical_operators_bool_test) {
    std::string expected_output{"true\nfalse\nfalse\ntrue\nfalse\ntrue\nfalse\ntrue\ntrue\nfalse\n"};
    std::string mock_file = R"(
def main() -> int {
    print((true and true) as string);     # true
    print((true and false) as string);    # false
    print((false or false) as string);    # false
    print((true or false) as string);     # true
    print((not true) as string);          # false
    print((not false) as string);         # true
    print(not (true and true) as string);     # false
    print(not (true and false) as string);    # true 
    print(not (false or false) as string);    # true
    print(not (true or false) as string);     # false
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

BOOST_AUTO_TEST_CASE(if_multiple_elseif_condition_test) {
    std::string expected_output{"second\nthird\nnone\n"};
    std::string mock_file = R"(
def test(x: int) -> none {
    if (x == 1) {
        print("first");
    } else if (x == 2) {
        print("second");
    } else if (x == 3) {
        print("third");
    } else {
        print("none");
    }
}

def main() -> int {
    test(2);
    test(3);
    test(99);
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
