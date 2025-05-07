#define BOOST_TEST_MODULE PARSER_TESTS
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/included/unit_test.hpp>
#include <queue>
#include <sstream>

#include "parser.hpp"
#include "parser_test_visitor.hpp"
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

/* -----------------------------------------------------------------------------*
 *                            TESTS USING MOCK LEXER                            *
 *------------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(test_minimal_main) {
    auto lexer = std::make_unique<MockLexer>();
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 0);
}
BOOST_AUTO_TEST_CASE(test_main_simple_return) {
    std::vector<Token> tokens = {
        Token{TokenType::T_DEF, Position(1, 1)},                 // def
        Token{TokenType::T_IDENTIFIER, Position(1, 5), "main"},  // main
        Token{TokenType::T_L_PAREN, Position(1, 9)},             // (
        Token{TokenType::T_R_PAREN, Position(1, 10)},            // )
        Token{TokenType::T_ARROW, Position(1, 12)},              // ->
        Token{TokenType::T_INT, Position(1, 15)},                // int
        Token{TokenType::T_L_BRACE, Position(1, 19)},            // {
        Token{TokenType::T_RETURN, Position(2, 5)},              // return
        Token{TokenType::T_LITERAL_INT, Position(2, 12), 0},     // 0
        Token{TokenType::T_SEMICOLON, Position(2, 13)},          // ;
        Token{TokenType::T_R_BRACE, Position(3, 1)},             // }
    };

    std::vector<std::pair<std::string, int>> expected_elements = {
        {"Program;[1:1]", 0},
        {"FunctionDefinition;[1:1]", 1},
        {"FunctionSignature;[1:1];type=function<none:int>,identifier=main", 2},
        {"CodeBlock;[1:19]", 2},
        {"ReturnStatement;[2:5]", 3},
        {"LiteralInt;[2:12];value=0", 4},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    ParserTestVisitor t_visit{};
    program->accept(t_visit);

    BOOST_CHECK(expected_elements == t_visit.elements);

    // Printer printer{};
    // program->accept(printer);
}

// def increment(mut i: int) -> none {
//     i = i + 1;
// }
BOOST_AUTO_TEST_CASE(test_increment_function) {
    std::vector<Token> tokens = {
        Token{TokenType::T_DEF, Position(1, 1)},                      // def
        Token{TokenType::T_IDENTIFIER, Position(1, 5), "increment"},  // increment
        Token{TokenType::T_L_PAREN, Position(1, 14)},                 // (
        Token{TokenType::T_MUT, Position(1, 15)},                     // mut
        Token{TokenType::T_IDENTIFIER, Position(1, 19), "i"},         // i
        Token{TokenType::T_COLON, Position(1, 20)},                   // :
        Token{TokenType::T_INT, Position(1, 22)},                     // int
        Token{TokenType::T_R_PAREN, Position(1, 25)},                 // )
        Token{TokenType::T_ARROW, Position(1, 27)},                   // ->
        Token{TokenType::T_NONE, Position(1, 30)},                    // none
        Token{TokenType::T_L_BRACE, Position(1, 35)},                 // {
        Token{TokenType::T_IDENTIFIER, Position(2, 5), "i"},          // i
        Token{TokenType::T_ASSIGN, Position(2, 7)},                   // =
        Token{TokenType::T_IDENTIFIER, Position(2, 9), "i"},          // i
        Token{TokenType::T_PLUS, Position(2, 11)},                    // +
        Token{TokenType::T_LITERAL_INT, Position(2, 13), 1},          // 1
        Token{TokenType::T_SEMICOLON, Position(2, 14)},               // ;
        Token{TokenType::T_R_BRACE, Position(3, 1)},                  // }
    };

    std::vector<std::pair<std::string, int>> expected_elements = {
        {"Program;[1:1]", 0},
        {"FunctionDefinition;[1:1]", 1},
        {"FunctionSignature;[1:1];type=function<mut int:none>,identifier=increment", 2},
        {"TypedIdentifier;[1:15];type=mut int,name=i", 3},
        {"CodeBlock;[1:35]", 2},
        {"AssignStatement;[2:5];to=i", 3},
        {"Addition;[2:9]", 4},
        {"Identifier;[2:9];name=i", 5},
        {"LiteralInt;[2:13];value=1", 5},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    ParserTestVisitor t_visit{};
    program->accept(t_visit);

    BOOST_CHECK(expected_elements == t_visit.elements);

    // Printer printer{};
    // program->accept(printer);
}

// def main() -> int {
//     let x: int = 4;
//     print(x);
//     return 0;
// }
BOOST_AUTO_TEST_CASE(test_main_simple_asgn_func_call) {
    std::vector<Token> tokens = {
        Token{TokenType::T_DEF, Position(1, 1)},                  // def
        Token{TokenType::T_IDENTIFIER, Position(1, 5), "main"},   // main
        Token{TokenType::T_L_PAREN, Position(1, 9)},              // (
        Token{TokenType::T_R_PAREN, Position(1, 10)},             // )
        Token{TokenType::T_ARROW, Position(1, 12)},               // ->
        Token{TokenType::T_INT, Position(1, 15)},                 // int
        Token{TokenType::T_L_BRACE, Position(1, 19)},             // {
        Token{TokenType::T_LET, Position(2, 5)},                  // let
        Token{TokenType::T_IDENTIFIER, Position(2, 9), "x"},      // x
        Token{TokenType::T_COLON, Position(2, 10)},               // :
        Token{TokenType::T_INT, Position(2, 12)},                 // int
        Token{TokenType::T_ASSIGN, Position(2, 16)},              // =
        Token{TokenType::T_LITERAL_INT, Position(2, 18), 4},      // 4
        Token{TokenType::T_SEMICOLON, Position(2, 19)},           // ;
        Token{TokenType::T_IDENTIFIER, Position(3, 5), "print"},  // print
        Token{TokenType::T_L_PAREN, Position(3, 10)},             // (
        Token{TokenType::T_IDENTIFIER, Position(3, 11), "x"},     // x
        Token{TokenType::T_R_PAREN, Position(3, 12)},             // )
        Token{TokenType::T_SEMICOLON, Position(3, 13)},           // ;
        Token{TokenType::T_RETURN, Position(4, 5)},               // return
        Token{TokenType::T_LITERAL_INT, Position(4, 12), 0},      // 0
        Token{TokenType::T_SEMICOLON, Position(4, 13)},           // ;
        Token{TokenType::T_R_BRACE, Position(5, 1)},              // }
    };

    std::vector<std::pair<std::string, int>> expected_elements = {
        {"Program;[1:1]", 0},
        {"FunctionDefinition;[1:1]", 1},
        {"FunctionSignature;[1:1];type=function<none:int>,identifier=main", 2},
        {"CodeBlock;[1:19]", 2},
        {"VariableDeclaration;[2:5]", 3},
        {"TypedIdentifier;[2:9];type=int,name=x", 4},
        {"LiteralInt;[2:18];value=4", 4},
        {"ExpressionStatement;[3:5]", 3},
        {"FunctionCall;[3:5]", 4},
        {"Identifier;[3:5];name=print", 5},
        {"Identifier;[3:11];name=x", 5},
        {"ReturnStatement;[4:5]", 3},
        {"LiteralInt;[4:12];value=0", 4},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    ParserTestVisitor t_visit{};
    program->accept(t_visit);

    BOOST_CHECK(expected_elements == t_visit.elements);

    // Printer printer{};
    // program->accept(printer);
}

// def invoke(fun: function<int, int:int>, a: int, b: int) -> int {
//     return fun(a, b);
// }
BOOST_AUTO_TEST_CASE(test_invoke_function) {
    std::vector<Token> tokens = {
        Token{TokenType::T_DEF, Position(1, 1)},                   // def
        Token{TokenType::T_IDENTIFIER, Position(1, 5), "invoke"},  // invoke
        Token{TokenType::T_L_PAREN, Position(1, 11)},              // (
        Token{TokenType::T_IDENTIFIER, Position(1, 12), "fun"},    // fun
        Token{TokenType::T_COLON, Position(1, 15)},                // :
        Token{TokenType::T_FUNCTION, Position(1, 17)},             // function
        Token{TokenType::T_LESS, Position(1, 25)},                 // <
        Token{TokenType::T_INT, Position(1, 26)},                  // int
        Token{TokenType::T_COMMA, Position(1, 29)},                // ,
        Token{TokenType::T_INT, Position(1, 31)},                  // int
        Token{TokenType::T_COLON, Position(1, 34)},                // :
        Token{TokenType::T_INT, Position(1, 35)},                  // int
        Token{TokenType::T_GREATER, Position(1, 38)},              // >
        Token{TokenType::T_COMMA, Position(1, 39)},                // ,
        Token{TokenType::T_IDENTIFIER, Position(1, 41), "a"},      // a
        Token{TokenType::T_COLON, Position(1, 42)},                // :
        Token{TokenType::T_INT, Position(1, 44)},                  // int
        Token{TokenType::T_COMMA, Position(1, 47)},                // ,
        Token{TokenType::T_IDENTIFIER, Position(1, 49), "b"},      // b
        Token{TokenType::T_COLON, Position(1, 50)},                // :
        Token{TokenType::T_INT, Position(1, 52)},                  // int
        Token{TokenType::T_R_PAREN, Position(1, 55)},              // )
        Token{TokenType::T_ARROW, Position(1, 57)},                // ->
        Token{TokenType::T_INT, Position(1, 60)},                  // int
        Token{TokenType::T_L_BRACE, Position(1, 64)},              // {
        Token{TokenType::T_RETURN, Position(2, 5)},                // return
        Token{TokenType::T_IDENTIFIER, Position(2, 12), "fun"},    // fun
        Token{TokenType::T_L_PAREN, Position(2, 15)},              // (
        Token{TokenType::T_IDENTIFIER, Position(2, 16), "a"},      // a
        Token{TokenType::T_COMMA, Position(2, 17)},                // ,
        Token{TokenType::T_IDENTIFIER, Position(2, 19), "b"},      // b
        Token{TokenType::T_R_PAREN, Position(2, 20)},              // )
        Token{TokenType::T_SEMICOLON, Position(2, 21)},            // ;
        Token{TokenType::T_R_BRACE, Position(3, 1)},               // }
    };

    std::vector<std::pair<std::string, int>> expected_elements = {
        {"Program;[1:1]", 0},
        {"FunctionDefinition;[1:1]", 1},
        {"FunctionSignature;[1:1];type=function<function<int,int:int>,int,int:int>,identifier=invoke", 2},
        {"TypedIdentifier;[1:12];type=function<int,int:int>,name=fun", 3},
        {"TypedIdentifier;[1:41];type=int,name=a", 3},
        {"TypedIdentifier;[1:49];type=int,name=b", 3},
        {"CodeBlock;[1:64]", 2},
        {"ReturnStatement;[2:5]", 3},
        {"FunctionCall;[2:12]", 4},
        {"Identifier;[2:12];name=fun", 5},
        {"Identifier;[2:16];name=a", 5},
        {"Identifier;[2:19];name=b", 5},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    ParserTestVisitor t_visit{};
    program->accept(t_visit);

    BOOST_CHECK(expected_elements == t_visit.elements);

    // Printer printer{};
    // program->accept(printer);
}

// def bind(fun: function<float, float, float:float>, arg1: float, arg2: float) -> function<float:float> {
//     return (arg1, arg2) >> fun;
// }
BOOST_AUTO_TEST_CASE(test_bind_function) {
    std::vector<Token> tokens = {
        Token{TokenType::T_DEF, Position(1, 1)},                  // def
        Token{TokenType::T_IDENTIFIER, Position(1, 5), "bind"},   // bind
        Token{TokenType::T_L_PAREN, Position(1, 9)},              // (
        Token{TokenType::T_IDENTIFIER, Position(1, 10), "fun"},   // fun
        Token{TokenType::T_COLON, Position(1, 13)},               // :
        Token{TokenType::T_FUNCTION, Position(1, 15)},            // function
        Token{TokenType::T_LESS, Position(1, 23)},                // <
        Token{TokenType::T_FLOAT, Position(1, 24)},               // float
        Token{TokenType::T_COMMA, Position(1, 29)},               // ,
        Token{TokenType::T_FLOAT, Position(1, 31)},               // float
        Token{TokenType::T_COMMA, Position(1, 37)},               // ,
        Token{TokenType::T_FLOAT, Position(1, 38)},               // float
        Token{TokenType::T_COLON, Position(1, 43)},               // :
        Token{TokenType::T_FLOAT, Position(1, 44)},               // float
        Token{TokenType::T_GREATER, Position(1, 49)},             // >
        Token{TokenType::T_COMMA, Position(1, 50)},               // ,
        Token{TokenType::T_IDENTIFIER, Position(1, 52), "arg1"},  // arg1
        Token{TokenType::T_COLON, Position(1, 53)},               // :
        Token{TokenType::T_FLOAT, Position(1, 58)},               // float
        Token{TokenType::T_COMMA, Position(1, 63)},               // ,
        Token{TokenType::T_IDENTIFIER, Position(1, 65), "arg2"},  // arg2
        Token{TokenType::T_COLON, Position(1, 69)},               // :
        Token{TokenType::T_FLOAT, Position(1, 71)},               // float
        Token{TokenType::T_R_PAREN, Position(1, 76)},             // )
        Token{TokenType::T_ARROW, Position(1, 78)},               // ->
        Token{TokenType::T_FUNCTION, Position(1, 81)},            // function
        Token{TokenType::T_LESS, Position(1, 89)},                // <
        Token{TokenType::T_FLOAT, Position(1, 90)},               // float
        Token{TokenType::T_COLON, Position(1, 95)},               // :
        Token{TokenType::T_FLOAT, Position(1, 96)},               // float
        Token{TokenType::T_GREATER, Position(1, 101)},            // >
        Token{TokenType::T_L_BRACE, Position(1, 103)},            // {
        Token{TokenType::T_RETURN, Position(2, 5)},               // return
        Token{TokenType::T_L_PAREN, Position(2, 12)},             // (
        Token{TokenType::T_IDENTIFIER, Position(2, 13), "arg1"},  // arg1
        Token{TokenType::T_COMMA, Position(2, 17)},               // ,
        Token{TokenType::T_IDENTIFIER, Position(2, 19), "arg2"},  // arg2
        Token{TokenType::T_R_PAREN, Position(2, 23)},             // )
        Token{TokenType::T_BIND_FRONT, Position(2, 25)},          // >>
        Token{TokenType::T_IDENTIFIER, Position(2, 28), "fun"},   // fun
        Token{TokenType::T_SEMICOLON, Position(2, 31)},           // ;
        Token{TokenType::T_R_BRACE, Position(3, 1)},              // }
    };

    std::vector<std::pair<std::string, int>> expected_elements = {
        {"Program;[1:1]", 0},
        {"FunctionDefinition;[1:1]", 1},
        {"FunctionSignature;[1:1];type=function<function<float,float,float:float>,float,float:function<float:float>>,"
         "identifier=bind",
         2},
        {"TypedIdentifier;[1:10];type=function<float,float,float:float>,name=fun", 3},
        {"TypedIdentifier;[1:52];type=float,name=arg1", 3},
        {"TypedIdentifier;[1:65];type=float,name=arg2", 3},
        {"CodeBlock;[1:103]", 2},
        {"ReturnStatement;[2:5]", 3},
        {"BindFront;[2:12]", 4},
        {"Identifier;[2:13];name=arg1", 5},
        {"Identifier;[2:19];name=arg2", 5},
        {"Identifier;[2:28];name=fun", 5},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    ParserTestVisitor t_visit{};
    program->accept(t_visit);

    BOOST_CHECK(expected_elements == t_visit.elements);

    // Printer printer{};
    // program->accept(printer);
}

// def nth_fibonacci(n: int) -> int {
//     if (n <= 1) {
//         return n;
//     }
//
//     return nth_fibonacci(n - 1) + nth_fibonacci(n - 2);
// }
//
// def main() -> int {
//     let n: int = 5;
//
//     print("For " + n as string + " sequence number is " + nth_fibonacci(n) as string);
//     return 0;
// }
BOOST_AUTO_TEST_CASE(test_fibonacci) {
    std::vector<Token> tokens = {
        // nth_fibonacci function
        Token{TokenType::T_DEF, Position(1, 1)},
        Token{TokenType::T_IDENTIFIER, Position(1, 5), "nth_fibonacci"},
        Token{TokenType::T_L_PAREN, Position(1, 18)},
        Token{TokenType::T_IDENTIFIER, Position(1, 19), "n"},
        Token{TokenType::T_COLON, Position(1, 20)},
        Token{TokenType::T_INT, Position(1, 22)},
        Token{TokenType::T_R_PAREN, Position(1, 25)},
        Token{TokenType::T_ARROW, Position(1, 27)},
        Token{TokenType::T_INT, Position(1, 30)},
        Token{TokenType::T_L_BRACE, Position(1, 34)},
        Token{TokenType::T_IF, Position(2, 5)},
        Token{TokenType::T_L_PAREN, Position(2, 8)},
        Token{TokenType::T_IDENTIFIER, Position(2, 9), "n"},
        Token{TokenType::T_LESS_EQUAL, Position(2, 11)},
        Token{TokenType::T_LITERAL_INT, Position(2, 14), 1},
        Token{TokenType::T_R_PAREN, Position(2, 15)},
        Token{TokenType::T_L_BRACE, Position(2, 17)},
        Token{TokenType::T_RETURN, Position(3, 9)},
        Token{TokenType::T_IDENTIFIER, Position(3, 16), "n"},
        Token{TokenType::T_SEMICOLON, Position(3, 17)},
        Token{TokenType::T_R_BRACE, Position(4, 5)},
        Token{TokenType::T_RETURN, Position(6, 5)},
        Token{TokenType::T_IDENTIFIER, Position(6, 12), "nth_fibonacci"},
        Token{TokenType::T_L_PAREN, Position(6, 25)},
        Token{TokenType::T_IDENTIFIER, Position(6, 26), "n"},
        Token{TokenType::T_MINUS, Position(6, 28)},
        Token{TokenType::T_LITERAL_INT, Position(6, 30), 1},
        Token{TokenType::T_R_PAREN, Position(6, 31)},
        Token{TokenType::T_PLUS, Position(6, 33)},
        Token{TokenType::T_IDENTIFIER, Position(6, 35), "nth_fibonacci"},
        Token{TokenType::T_L_PAREN, Position(6, 48)},
        Token{TokenType::T_IDENTIFIER, Position(6, 49), "n"},
        Token{TokenType::T_MINUS, Position(6, 51)},
        Token{TokenType::T_LITERAL_INT, Position(6, 53), 2},
        Token{TokenType::T_R_PAREN, Position(6, 54)},
        Token{TokenType::T_SEMICOLON, Position(6, 55)},
        Token{TokenType::T_R_BRACE, Position(7, 1)},

        // main function
        Token{TokenType::T_DEF, Position(9, 1)},
        Token{TokenType::T_IDENTIFIER, Position(9, 5), "main"},
        Token{TokenType::T_L_PAREN, Position(9, 9)},
        Token{TokenType::T_R_PAREN, Position(9, 10)},
        Token{TokenType::T_ARROW, Position(9, 12)},
        Token{TokenType::T_INT, Position(9, 15)},
        Token{TokenType::T_L_BRACE, Position(9, 19)},
        Token{TokenType::T_LET, Position(10, 5)},
        Token{TokenType::T_IDENTIFIER, Position(10, 9), "n"},
        Token{TokenType::T_COLON, Position(10, 10)},
        Token{TokenType::T_INT, Position(10, 12)},
        Token{TokenType::T_ASSIGN, Position(10, 16)},
        Token{TokenType::T_LITERAL_INT, Position(10, 18), 5},
        Token{TokenType::T_SEMICOLON, Position(10, 19)},
        Token{TokenType::T_IDENTIFIER, Position(12, 5), "print"},
        Token{TokenType::T_L_PAREN, Position(12, 10)},
        Token{TokenType::T_LITERAL_STRING, Position(12, 11), "For "},
        Token{TokenType::T_PLUS, Position(12, 18)},
        Token{TokenType::T_IDENTIFIER, Position(12, 20), "n"},
        Token{TokenType::T_AS, Position(12, 22)},
        Token{TokenType::T_STRING, Position(12, 25)},
        Token{TokenType::T_PLUS, Position(12, 32)},
        Token{TokenType::T_LITERAL_STRING, Position(12, 34), " sequence number is "},
        Token{TokenType::T_PLUS, Position(12, 57)},
        Token{TokenType::T_IDENTIFIER, Position(12, 59), "nth_fibonacci"},
        Token{TokenType::T_L_PAREN, Position(12, 72)},
        Token{TokenType::T_IDENTIFIER, Position(12, 73), "n"},
        Token{TokenType::T_R_PAREN, Position(12, 74)},
        Token{TokenType::T_AS, Position(12, 76)},
        Token{TokenType::T_STRING, Position(12, 79)},
        Token{TokenType::T_R_PAREN, Position(12, 85)},
        Token{TokenType::T_SEMICOLON, Position(12, 86)},
        Token{TokenType::T_RETURN, Position(13, 5)},
        Token{TokenType::T_LITERAL_INT, Position(13, 12), 0},
        Token{TokenType::T_SEMICOLON, Position(13, 13)},
        Token{TokenType::T_R_BRACE, Position(14, 1)},
    };

    std::vector<std::pair<std::string, int>> expected_elements = {
        {"Program;[1:1]", 0},
        {"FunctionDefinition;[1:1]", 1},
        {"FunctionSignature;[1:1];type=function<int:int>,identifier=nth_fibonacci", 2},
        {"TypedIdentifier;[1:19];type=int,name=n", 3},
        {"CodeBlock;[1:34]", 2},
        {"IfStatement;[2:5]", 3},
        {"LessEqual;[2:9]", 4},
        {"Identifier;[2:9];name=n", 5},
        {"LiteralInt;[2:14];value=1", 5},
        {"CodeBlock;[2:17]", 4},
        {"ReturnStatement;[3:9]", 5},
        {"Identifier;[3:16];name=n", 6},
        {"ReturnStatement;[6:5]", 3},
        {"Addition;[6:12]", 4},
        {"FunctionCall;[6:12]", 5},
        {"Identifier;[6:12];name=nth_fibonacci", 6},
        {"Subtraction;[6:26]", 6},
        {"Identifier;[6:26];name=n", 7},
        {"LiteralInt;[6:30];value=1", 7},
        {"FunctionCall;[6:35]", 5},
        {"Identifier;[6:35];name=nth_fibonacci", 6},
        {"Subtraction;[6:49]", 6},
        {"Identifier;[6:49];name=n", 7},
        {"LiteralInt;[6:53];value=2", 7},
        {"FunctionDefinition;[9:1]", 1},
        {"FunctionSignature;[9:1];type=function<none:int>,identifier=main", 2},
        {"CodeBlock;[9:19]", 2},
        {"VariableDeclaration;[10:5]", 3},
        {"TypedIdentifier;[10:9];type=int,name=n", 4},
        {"LiteralInt;[10:18];value=5", 4},
        {"ExpressionStatement;[12:5]", 3},
        {"FunctionCall;[12:5]", 4},
        {"Identifier;[12:5];name=print", 5},
        {"Addition;[12:11]", 5},
        {"Addition;[12:11]", 6},
        {"Addition;[12:11]", 7},
        {R"(LiteralString;[12:11];value="For ")", 8},
        {"TypeCast;[12:20];to_type=string", 8},
        {"Identifier;[12:20];name=n", 9},
        {R"(LiteralString;[12:34];value=" sequence number is ")", 7},
        {"TypeCast;[12:59];to_type=string", 6},
        {"FunctionCall;[12:59]", 7},
        {"Identifier;[12:59];name=nth_fibonacci", 8},
        {"Identifier;[12:73];name=n", 8},
        {"ReturnStatement;[13:5]", 3},
        {"LiteralInt;[13:12];value=0", 4},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 2);

    ParserTestVisitor t_visit{};
    program->accept(t_visit);

    BOOST_CHECK(expected_elements == t_visit.elements);

    // Printer printer{};  // for reference
    // program->accept(printer);
}

// BOOST_AUTO_TEST_CASE(test_fail) {
//     BOOST_CHECK_EQUAL(1, 0);
// }
