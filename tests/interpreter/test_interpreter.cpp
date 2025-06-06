#define BOOST_TEST_MODULE INTERPRETER_TESTS
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>

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
    std::string expected_output{"false\ntrue\nfalse\ntrue\nfalse\ntrue\nfalse\ntrue\n"};
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

    print_bool(false as bool); # false
    print_bool(true as bool); # true
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

// FUNCTION COMPOSITION, FUNC COMP
BOOST_AUTO_TEST_CASE(func_comp_single_test) {
    std::string expected_output{"4\n99\n"};
    std::string mock_file = R"(
def int_to_str(a: int) -> string {
    return a as string;
}

def main() -> int {
    let print_int: function<int:none> = int_to_str & print;
    print_int(4);
    (int_to_str & print)(99); # wolane dynamicznie tez dziala

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

BOOST_AUTO_TEST_CASE(func_comp_chained_test) {
    std::string expected_output{"8\n2.5\n"};
    std::string mock_file = R"(
def mul2_int(a: int) -> int {
    return a * 2;
}

def to_flt(a: int) -> float {
    return a as float;
}

def div4_flt(f: float) -> float {
    return f / 4 as float;
}

def int_to_str(a: int) -> string {
    return a as string;
}

def flt_to_str(f: float) -> string {
    return f as string;
}

def main() -> int {
    let print_2mul_int: function<int:none> =  mul2_int & int_to_str & print;
    print_2mul_int(4);

    let foo: function<int:float> = mul2_int & to_flt & div4_flt;

    (foo & flt_to_str & print)(5);


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

BOOST_AUTO_TEST_CASE(bind_front_simple_test) {
    std::string expected_output{"7\n"};
    std::string mock_file = R"(
def add(a: int, b: int) -> int {
    return a + b;
}

def main() -> int {
    let add5: function<int:int> = (5) >> add;
    print(add5(2) as string); # 5 + 2 = 7
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

BOOST_AUTO_TEST_CASE(bind_front_multiple_args_test) {
    std::string expected_output{"15\n"};
    std::string mock_file = R"(
def sum3(a: int, b: int, c: int) -> int {
    return a + b + c;
}

def main() -> int {
    let add10: function<int:int> = (5, 5) >> sum3;
    print(add10(5) as string); # 5 + 5 + 5 = 15
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

BOOST_AUTO_TEST_CASE(bind_front_with_type_cast_test) {
    std::string expected_output{"foo42\n"};
    std::string mock_file = R"(
def concat(a: string, b: int) -> string {
    return a + b as string;
}

def main() -> int {
    let foo42: function<none:string> = ("foo", 42) >> concat;
    print(foo42());
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

BOOST_AUTO_TEST_CASE(bind_front_with_func_comp_test) {
    std::string expected_output{"hello there anakin\nhello there lukasz\n"};
    std::string mock_file = R"(
def get_greeter() -> function<string:none> {
    return (hello() + " ", "there ") >> three_concat & print;
}

def three_concat(a: string, b: string, c:string) -> string {
    return a + b + c;
}

def hello() -> string{
    return "hello";
}

def main() -> int {
    let anakin_greeter: function<none:none> = ("anakin") >> get_greeter();
    anakin_greeter();
    get_greeter()("lukasz");
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

BOOST_AUTO_TEST_CASE(for_loop_simple_test) {
    std::string expected_output{"0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n"};
    std::string mock_file = R"(
def main() -> int {
    for (i: int = 0; i < 10;  i = i+1){
        print(i as string);
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

BOOST_AUTO_TEST_CASE(for_loop_continue_test) {
    std::string expected_output{"0\n1\n2\n3\n5\n6\n7\n8\n9\n"};
    std::string mock_file = R"(
def main() -> int {
    let skipped: int = 4;
    for (i: int = 0; i < 10;  i = i+1){
        if (i == skipped){
            continue;
        }
        print(i as string);
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

BOOST_AUTO_TEST_CASE(for_loop_break_test) {
    std::string expected_output{"0\n1\n2\n3\n"};
    std::string mock_file = R"(
def main() -> int {
    let breaking: int = 4;
    for (i: int = 0; i < 10;  i = i+1){
        if (i == breaking){
            break;
        }
        print(i as string);
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

BOOST_AUTO_TEST_CASE(for_loop_with_return_no_val_test) {
    std::string expected_output{"0\n1\n2\n3\n4\n5\n6\nin main\n"};
    std::string mock_file = R"(
def foo(a: int) -> none {
    for (i: int = 0; i < 10;  i = i+1){
        if (i * 3 > a ){
            return;
        }
        print(i as string);
    }
}


def main() -> int {
    foo(20);
    print("in main");
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

BOOST_AUTO_TEST_CASE(for_loop_with_return_val_test) {
    std::string expected_output{"0\n1\n2\n3\n4\nval not reached\n0\n1\n2\nval reached\n"};
    std::string mock_file = R"(
def foo(a: int) -> string {
    for (i: int = 0; i < 5;  i = i+1){
        if ( a == i ){
            return "val reached";
        }
        print(i as string);
    }
    return "val not reached";
}


def main() -> int {
    print(foo(5));
    print(foo(3));
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

BOOST_AUTO_TEST_CASE(for_loop_shadowing_test) {
    std::string expected_output{
        "outside loop i is: 4.5\ninside loop i is: 0\ninside loop i is: 1\noutside loop i is: 4.5\n"};
    std::string mock_file = R"(
def main() -> int {
    let i: float = 4.5;
    let position: string = "outside loop";

    print(position + " i is: " + i as string);

    for (i: int = 0; i < 2;  i = i+1){
        let position: string = "inside loop";
        print(position + " i is: " + i as string);
    }

    print(position + " i is: " + i as string);

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

BOOST_AUTO_TEST_CASE(block_shadowing_test) {
    std::string expected_output{"outer: 10\ninner: 20\ntrue\nouter: 10\n"};
    std::string mock_file = R"(
def main() -> int {
    let x: int = 10;
    let access: bool = true;

    print("outer: " + x as string);

    {
        let x: int = 20;
        print("inner: " + x as string);
        print(access as string); # to show that parent scope can be referenced
    }

    print("outer: " + x as string);

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

BOOST_AUTO_TEST_CASE(is_int_builtin_test) {
    std::string expected_output{"true\nfalse\ntrue\nfalse\n"};
    std::string mock_file = R"(
def main() -> int {
    print(is_int("123") as string);      # true
    print(is_int("12.3") as string);     # false
    print(is_int("-42") as string);      # true
    print(is_int("abc") as string);      # false
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

BOOST_AUTO_TEST_CASE(is_float_builtin_test) {
    std::string expected_output{"true\ntrue\ntrue\nfalse\n"};
    std::string mock_file = R"(
def main() -> int {
    print(is_float("12.3") as string);   # true
    print(is_float("-42.0") as string);  # true
    print(is_float("123") as string);    # true - int value can be read as float
    print(is_float("abc") as string);    # false
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

BOOST_AUTO_TEST_CASE(lower_builtin_test) {
    std::string expected_output{"abc\nabc123\nabc def\n"};
    std::string mock_file = R"(
def main() -> int {
    print(lower("ABC"));
    print(lower("AbC123"));
    print(lower("ABC DEF"));
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

BOOST_AUTO_TEST_CASE(upper_builtin_test) {
    std::string expected_output{"ABC\nABC123\nABC DEF\n"};
    std::string mock_file = R"(
def main() -> int {
    print(upper("abc"));
    print(upper("AbC123"));
    print(upper("abc def"));
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

BOOST_AUTO_TEST_CASE(capitalized_builtin_test) {
    std::string expected_output{"Abc\nA\nA\nAbc def\n"};
    std::string mock_file = R"(
def main() -> int {
    print(capitalized("abc"));
    print(capitalized("A"));
    print(capitalized("a"));
    print(capitalized("abc DEF"));
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

// EXCEPTION TESTS

namespace bdata = boost::unit_test::data;

std::vector<std::string> return_type_mismatch_exception_cases = {
    R"(def main() -> int { return -0.4; })",
    R"(def main() -> int { return true; })",
    R"(def main() -> int { return "fdfdf"; })",
    R"(def main() -> int { return; })",
    R"(def main() -> int { return print; })",
    R"(
def to_string(i: int) -> string {return;}

def main() -> int{
    to_string(4);
    return 0;
}
    )",
};
BOOST_DATA_TEST_CASE(return_type_mismatch_exception_test, bdata::make(return_type_mismatch_exception_cases),
                     mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);

    BOOST_CHECK_THROW(program->accept(interpreter), ReturnTypeMismatchException);
}

std::vector<std::string> assign_type_mismatch_exception_cases = {
    R"(def main() -> int { let x: int = 3.14; return 0; })",
    R"(def main() -> int { let x: int = "abc"; return 0; })",
    R"(def main() -> int { let x: int = true; return 0; })",
    R"(def main() -> int { let x: float = "abc"; return 0; })",
    R"(def main() -> int { let x: bool = 123; return 0; })",
    R"(def main() -> int { let x: string = 3.14; return 0; })",
    R"(def main() -> int { let x: string = false; return 0; })",
    R"(def main() -> int { let x: int = print; return 0; })",
    R"(def main() -> int { let x: int = print(4 as string); return 0; })",
    R"(
def foo() -> int { return 1; }
def main() -> int { let x: string = foo(); return 0; }
    )",
    R"(
def main() -> int {
    let mut x: int = 5;
    x = "abc";
    return 0;
}
    )",
    R"(
def main() -> int {
    let mut x: float = 1.5;
    x = false;
    return 0;
}
    )",
};

BOOST_DATA_TEST_CASE(assign_type_mismatch_exception_test, bdata::make(assign_type_mismatch_exception_cases),
                     mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), AssignTypeMismatchException);
}

std::vector<std::string> cannot_cast_exception_cases = {
    R"(def main() -> int { let x: int = "abc" as int; return 0; })",
    R"(def main() -> int { let x: int = "" as int; return 0; })",
    R"(def main() -> int { let x: int = "12.3.4" as int; return 0; })",
    R"(def main() -> int { let x: int = "99999999999999999999" as int; return 0; })",
    R"(def main() -> int { let x: int = print as int; return 0; })",
    R"(def main() -> int { let x: float = "abc" as float; return 0; })",
    R"(def main() -> int { let x: float = "" as float; return 0; })",
    R"(def main() -> int { let x: float = "12.3.4" as float; return 0; })",
    R"(def main() -> int { let x: float = upper as float; return 0; })",
    R"(def main() -> int { let x: int = print as function<none:none>; return 0; })",
    R"(def main() -> int { let x: float = print as float; return 0; })",
    R"(def main() -> int { let x: bool = print as bool; return 0; })",
    R"(def main() -> int { let x: string = print as string; return 0; })",
    R"(
def main() -> int {
    let mut x: int = 0;
    x = "true" as int;
    return 0;
}
    )",
    R"(
def main() -> int {
    let mut x: float = 3.4;
    x = "false" as float;
    return 0;
}
    )",
};

BOOST_DATA_TEST_CASE(cannot_cast_exception_test, bdata::make(cannot_cast_exception_cases), mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), CannotCastException);
}

std::vector<std::string> unknown_identifier_exception_cases = {
    R"(def main() -> int { print(x); return 0; })",
    R"(def main() -> int { let a: int = 5; print(b); return 0; })",
    R"(def main() -> int { x = 5; return 0; })",
    R"(def main() -> int { let a: int = y + 2; return 0; })",
    R"(
def foo() -> int {
    return z;
}
def main() -> int {
    foo();
    return 0;
}
    )",
    R"(
def main() -> int {
    if (true) {
        print(y);
    }
    return 0;
}
    )",
    R"(
def main() -> int {
    for (i: int = 0; i < 1; i = i+1) {
        print(j);
    }
    return 0;
}
    )",
};

BOOST_DATA_TEST_CASE(unknown_identifier_exception_test, bdata::make(unknown_identifier_exception_cases), mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), UnknownIdentifierException);
}

std::vector<std::string> arg_types_not_matching_exception_cases = {
    R"(
def foo(a: int) -> int { return a; }
def main() -> int { foo(); return 0; }
    )",
    R"(
def foo(a: int) -> int { return a; }
def main() -> int { foo("abc"); return 0; }
    )",
    R"(
def foo(a: int, b: float) -> int { return a; }
def main() -> int { foo(1); return 0; }
    )",
    R"(
def foo(a: int, b: float) -> int { return a; }
def main() -> int { foo(1, "abc"); return 0; }
    )",
    R"(
def foo(a: int) -> int { return a; }
def main() -> int { foo(1, 2); return 0; }
    )",
    R"(
def bar(a: string, b: bool) -> int { return 0; }
def main() -> int { bar(123, 456); return 0; }
    )",
    R"(
def bar(a: string, b: bool) -> int { return 0; }
def main() -> int { bar(); return 0; }
    )",
    R"(
def bar(a: string, b: bool) -> int { return 0; }
def main() -> int { bar("ok"); return 0; }
    )",
    R"(
def main() -> int {((4) >> print)(); return 0;} 
    )",
};

BOOST_DATA_TEST_CASE(arg_types_not_matching_exception_test, bdata::make(arg_types_not_matching_exception_cases),
                     mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), ArgTypesNotMatchingException);
}

std::vector<std::string> too_many_args_to_bind_exception_cases = {
    R"(
def add(a: int, b: int) -> int { return a + b; }
def main() -> int {
    let add5: function<none:int> = (5, 6, 7) >> add; # add przyjmuje tylko 2 argumenty
    return 0;
}
    )",
    R"(
def foo(a: int) -> int { return a; }
def main() -> int {
    let f : function<none:int> = (1, 2) >> foo;
    return 0;
}
    )",
    R"(
def bar(a: int, b: int) -> int { return a + b; }
def main() -> int {
    let f: function<none:int> = (1, 2, 3, 4) >> bar;
    return 0;
}
    )",
    R"(
def main() -> int {
    let f: function<none:none> = (1, 2, 3) >> print;
    return 0;
}
    )",
};

BOOST_DATA_TEST_CASE(too_many_args_to_bind_exception_test, bdata::make(too_many_args_to_bind_exception_cases),
                     mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), TooManyArgsToBindException);
}

std::vector<std::string> cant_perform_operation_exception_cases = {
    R"(def main() -> int { print + upper; return 0; })",
    R"(def main() -> int { print - upper; return 0; })",
    R"(def main() -> int { lower * upper; return 0; })",
    R"(def main() -> int { round / main; return 0; })",
    R"(def main() -> int { round and main; return 0; })",
    R"(def main() -> int { round or main; return 0; })",
    R"(def main() -> int { round == main; return 0; })",
    R"(def main() -> int { round < main; return 0; })",
    R"(def main() -> int { not input; return 0; })",
    R"(def main() -> int { -print; return 0; })",
    R"(
def main() -> int {
    (true + false);
    return 0;
}
    )",
    R"(
def add1(a:int, b:int) -> int {return a + b;}
def sub1(a:int, b:int) -> int {return a - b;}

def main() -> int {
    (add1 + sub1);
    return 0;
}
    )",
    R"(
def main() -> int {
    ("abc" - "def");
    return 0;
}
    )",
    R"(
def main() -> int {
    (true - false);
    return 0;
}
    )",
    R"(
def add1(a:int, b:int) -> int {return a + b;}
def sub1(a:int, b:int) -> int {return a - b;}

def main() -> int {
    (add1 - sub1);
    return 0;
}
    )",
    R"(
def main() -> int {
    ("abc" / "def");
    return 0;
}
    )",
    R"(
def main() -> int {
    (true / true);
    return 0;
}
    )",
    R"(
def add1(a:int, b:int) -> int {return a + b;}
def sub1(a:int, b:int) -> int {return a - b;}

def main() -> int {
    (add1 / sub1);
    return 0;
}
    )",
    R"(
def main() -> int {
    ("abc" / "def");
    return 0;
}
    )",
    R"(
def main() -> int {
    (true * true);
    return 0;
}
    )",
    R"(
def add1(a:int, b:int) -> int {return a + b;}
def sub1(a:int, b:int) -> int {return a - b;}

def main() -> int {
    (add1 * sub1);
    return 0;
}
    )",
};

BOOST_DATA_TEST_CASE(cant_perform_operation_exception_test, bdata::make(cant_perform_operation_exception_cases),
                     mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), CantPerformOperationException);
}

std::vector<std::string> required_function_exception_cases = {
    R"(
def main() -> int {
    4();
    return 0;
}
    )",
    R"(
def main() -> int {
    "abc"();
    return 0;
}
    )",
    R"(
def main() -> int {
    4.5 & 9.0;
    return 0;
}
    )",
    R"(
def main() -> int {
    "foo" & "bar";
    return 0;
}
    )",
    R"(
def main() -> int {
    (4) >> (4+3);
    return 0;
}
    )",
    R"(
def main() -> int {
    ("foo") >> ("bar");
    return 0;
}
    )",
};

BOOST_DATA_TEST_CASE(required_function_exception_test, bdata::make(required_function_exception_cases), mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), RequiredFunctionException);
}

std::vector<std::string> cant_assign_to_immutable_exception_cases = {
    // Przypisanie do niemutowalnej zmiennej
    R"(
def main() -> int {
    let x: int = 5;
    x = 10;
    return 0;
}
    )",
    R"(
def main() -> int {
    let y: float = 3.14;
    y = 2.71;
    return 0;
}
    )",
    R"(
def main() -> int {
    let s: string = "abc";
    s = "def";
    return 0;
}
    )",
    R"(
def main() -> int {
    let b: bool = true;
    b = false;
    return 0;
}
    )",
    R"(
def foo(a: int) -> none {
    a = 42;
}
def main() -> int {
    foo(5);
    return 0;
}
    )",
};

BOOST_DATA_TEST_CASE(cant_assign_to_immutable_exception_test, bdata::make(cant_assign_to_immutable_exception_cases),
                     mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), CantAssignToImmutableException);
}

std::vector<std::string> loop_stmt_outside_loop_exception_cases = {
    // break poza pętlą
    R"(
def main() -> int {
    break;
    return 0;
}
    )",
    R"(
def main() -> int {
    continue;
    return 0;
}
    )",
    R"(
def main() -> int {
    if (true) {
        break;
    }
    return 0;
}
    )",
    R"(
def main() -> int {
    if (1 == 1) {
        continue;
    }
    return 0;
}
    )",
    R"(
def foo() -> none {
    break;
}
def main() -> int {
    foo();
    return 0;
}
    )",
    R"(
def bar() -> none {
    continue;
}
def main() -> int {
    bar();
    return 0;
}
    )",
};

BOOST_DATA_TEST_CASE(loop_stmt_outside_loop_exception_test, bdata::make(loop_stmt_outside_loop_exception_cases),
                     mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), LoopStmtOutsideLoopException);
}

std::vector<std::string> expected_evaluable_expr_exception_cases = {
    R"(
def foo() -> none { }
def main() -> int {
    print(foo() + 5);
    return 0;
}
    )",
    R"(
def foo() -> none { }
def main() -> int {
    print(-foo());
    return 0;
}
    )",
    R"(
def foo() -> none { }
def bar(a: int) -> int { return a; }
def main() -> int {
    bar(foo());
    return 0;
}
    )",
    R"(
def foo() -> none { }
def bar(a: int, b: int) -> int { return a + b; }
def main() -> int {
    bar(1, foo());
    return 0;
}
    )",
    R"(
def foo() -> none { }
def bar() -> none { }
def main() -> int {
    print(foo() + bar());
    return 0;
}
    )",
    R"(
def foo() -> none { }
def main() -> int {
    print(not foo());
    return 0;
}
    )",
};

BOOST_DATA_TEST_CASE(expected_evaluable_expr_exception_test, bdata::make(expected_evaluable_expr_exception_cases),
                     mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), ExpectedEvaluableExprException);
}

std::vector<std::string> binary_expr_type_mismatch_exception_cases = {
    R"(
def main() -> int {
    print(5 + 3.14);
    return 0;
}
    )",
    R"(
def main() -> int {
    print(2.71 - 2);
    return 0;
}
    )",
    R"(
def main() -> int {
    print("abc" * 3);
    return 0;
}
    )",
    R"(
def main() -> int {
    print(true + "abc");
    return 0;
}
    )",
    R"(
def main() -> int {
    print(42 == "42");
    return 0;
}
    )",
    R"(
def main() -> int {
    print(3.14 < false);
    return 0;
}
    )",
    R"(
def main() -> int {
    print("abc" > 5);
    return 0;
}
    )",
    R"(
def main() -> int {
    print(true != 1.0);
    return 0;
}
    )",
};

BOOST_DATA_TEST_CASE(binary_expr_type_mismatch_exception_test, bdata::make(binary_expr_type_mismatch_exception_cases),
                     mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), BinaryExprTypeMismatchException);
}

std::vector<std::string> already_defined_exception_cases = {
    // Dwie zmienne o tej samej nazwie w tym samym bloku
    R"(
def main() -> int {
    let x: int = 5;
    let x: int = 10;
    return 0;
}
    )",
    // Dwie zmienne o tej samej nazwie w tym samym bloku (różne typy)
    R"(
def main() -> int {
    let y: int = 1;
    let y: string = "abc";
    return 0;
}
    )",
    // Dwie funkcje o tej samej nazwie
    R"(
def foo() -> int { return 1; }
def foo() -> int { return 2; }
def main() -> int { return foo(); }
    )",
    R"(
def bar() -> int { return 1; }
def main() -> int {
    let bar: int = 5;
    return bar;
}
    )",
    R"(
def main() -> int {
    let z: int = 1;
    {
        let z: int = 2; # ok
        let z: int = 3; # err
    }
    return 0;
}
    )",
};

BOOST_DATA_TEST_CASE(already_defined_exception_test, bdata::make(already_defined_exception_cases), mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), AlreadyDefinedException);
}

std::vector<std::string> invalid_func_t_for_composition_exception_cases = {
    R"(
def f1(a: int) -> int { return a + 1; }
def f2() -> int { return 42; }
def main() -> int {
    let comp: function<int:int> = f1 & f2;
    return 0;
}
    )",
    R"(
def f1(a: int) -> int { return a + 1; }
def f2(a: int, b: int) -> int { return a + b; }
def main() -> int {
    let comp: function<int:int> = f1 & f2;
    return 0;
}
    )",
    R"(
def f1(a: int) -> string { return a as string; }
def f2(a: int) -> int { return a + 1; }
def main() -> int {
    let comp: function<string:int> = f1 & f2;
    return 0;
}
    )",
    R"(
def f1(a: int) -> int { return a + 1; }
def f2(a: string) -> int { return 42; }
def main() -> int {
    let comp: function<int:string> = f1 & f2;
    return 0;
}
    )",
    R"(
def f1(a: int) -> int { return a + 1; }
def f2() -> string { return "foo"; }
def main() -> int {
    let comp: function<int: string> = f1 & f2;
    return 0;
}
    )",
    R"(
def f1(a: int) -> int { return a + 1; }
def f2(a: int, b: string) -> string { return b; }
def main() -> int {
    let comp: function<int:string> = f1 & f2;
    return 0;
}
    )",
};

BOOST_DATA_TEST_CASE(invalid_func_t_for_composition_exception_test,
                     bdata::make(invalid_func_t_for_composition_exception_cases), mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), InvalidFucTForCompositionExeption);
}

std::vector<std::string> condition_must_be_bool_exception_cases = {
    R"(
def main() -> int {
    if (5) {
        print("should not work");
    }
    return 0;
}
    )",
    R"(
def main() -> int {
    if ("abc") {
        print("should not work");
    }
    return 0;
}
    )",
    R"(
def main() -> int {
    if (3.14) {
        print("should not work");
    }
    return 0;
}
    )",
    R"(
def main() -> int {
    for (i: int = 0; 1; i = i+1) {
        print(i as string);
    }
    return 0;
}
    )",
    R"(
def main() -> int {
    for (i: int = 0; "abc"; i = i+1) {
        print(i as string);
    }
    return 0;
}
    )",
};

BOOST_DATA_TEST_CASE(condition_must_be_bool_exception_test, bdata::make(condition_must_be_bool_exception_cases),
                     mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), ConditionMustBeBoolException);
}

std::vector<std::string> int_overflow_exception_cases = {
    R"(
def main() -> int {
    let x: int = 2147483647;
    let y: int = x + 1;
    return 0;
}
    )",
    R"(
def main() -> int {
    let x: int = 1073741824;
    let y: int = x * 4;
    return 0;
}
    )",
    R"(
def main() -> int {
    let x: int = -2147483648;
    let y: int = x - 1;
    return 0;
}
    )",
};

BOOST_DATA_TEST_CASE(int_overflow_exception_test, bdata::make(int_overflow_exception_cases), mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), IntOverflowException);
}

std::vector<std::string> div_by_zero_exception_cases = {
    // Dzielenie int przez zero
    R"(
def main() -> int {
    let x: int = 5 / 0;
    return 0;
}
    )",
    // Dzielenie float przez zero
    R"(
def main() -> int {
    let x: float = 3.14 / 0.0;
    return 0;
}
    )",
    // Dzielenie zmiennej przez zero
    R"(
def main() -> int {
    let y: int = 0;
    let x: int = 10 / y;
    return 0;
}
    )",
};

BOOST_DATA_TEST_CASE(div_by_zero_exception_test, bdata::make(div_by_zero_exception_cases), mock_file) {
    Interpreter interpreter{};
    auto program = get_program(mock_file);
    BOOST_CHECK_THROW(program->accept(interpreter), DivByZeroException);
}
