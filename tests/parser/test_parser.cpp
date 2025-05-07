#define BOOST_TEST_MODULE PARSER_TESTS
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/included/unit_test.hpp>
#include <queue>
#include <sstream>

#include "parser.hpp"
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
template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2> pair) {
    os << std::format("({}, {})", pair.first, pair.second);
    return os;
}

// class for testing, visits program and builds vector of pairs (std::string element, int nest_level)
// vector descibed below with example
struct TestVisitor : public Visitor {
    void visit(const Program& program) override {
        elements.emplace_back(_get_name_position_str(program, "Program"), _nest_level);
        _NestGuard guard{_nest_level};
        std::ranges::for_each(program.statements, [this](const auto& stmnt) { stmnt->accept(*this); });
    }

    void visit(const ContinueStatement& continue_stmnt) override {
        elements.emplace_back(_get_name_position_str(continue_stmnt, "ContinueStatement"), _nest_level);
    }

    void visit(const BreakStatement& break_stmnt) override {
        elements.emplace_back(_get_name_position_str(break_stmnt, "BreakStatement"), _nest_level);
    }

    void visit(const ReturnStatement& return_stmnt) override {
        elements.emplace_back(_get_name_position_str(return_stmnt, "ReturnStatement"), _nest_level);
        _NestGuard guard{_nest_level};
        return_stmnt.expression->accept(*this);
    }

    void visit(const VariableDeclaration& var_decl) override {
        elements.emplace_back(_get_name_position_str(var_decl, "VariableDeclaration"), _nest_level);
        _NestGuard guard{_nest_level};
        var_decl.typed_identifier->accept(*this);
        var_decl.assigned_expression->accept(*this);
    }

    void visit(const CodeBlock& code_block) override {
        elements.emplace_back(_get_name_position_str(code_block, "CodeBlock"), _nest_level);
        _NestGuard guard{_nest_level};
        std::ranges::for_each(code_block.statements, [this](const auto& stmnt) { stmnt->accept(*this); });
    }

    void visit(const IfStatement& if_stmnt) override {
        elements.emplace_back(_get_name_position_str(if_stmnt, "IfStatement"), _nest_level);
        _NestGuard guard{_nest_level};
        if_stmnt.condition->accept(*this);
        if_stmnt.body->accept(*this);
        std::ranges::for_each(if_stmnt.else_ifs, [this](const auto& stmnt) { stmnt->accept(*this); });
        if (if_stmnt.else_body) {
            if_stmnt.else_body->accept(*this);
        }
    }

    void visit(const ElseIf& else_if) override {
        elements.emplace_back(_get_name_position_str(else_if, "ElseIf"), _nest_level);
        _NestGuard guard{_nest_level};
        else_if.condition->accept(*this);
        else_if.body->accept(*this);
    }

    void visit(const AssignStatement& asgn_stmnt) override {
        std::string assign_str{_get_name_position_str(asgn_stmnt, "AssignStatement") +
                               std::format(";to={}", asgn_stmnt.identifier)};
        elements.emplace_back(assign_str, _nest_level);
        _NestGuard guard{_nest_level};
        asgn_stmnt.expr->accept(*this);
    }

    void visit(const ExpressionStatement& expr_stmnt) override {
        elements.emplace_back(_get_name_position_str(expr_stmnt, "ExpressionStatement"), _nest_level);
        _NestGuard guard{_nest_level};
        expr_stmnt.expr->accept(*this);
    }

    void visit(const FunctionDefinition& func_def) override {
        elements.emplace_back(_get_name_position_str(func_def, "FunctionDefinition"), _nest_level);
        _NestGuard guard{_nest_level};
    }

    void visit(const FunctionSignature& func_sig) override {
        std::string func_sig_str{_get_name_position_str(func_sig, "FunctionSignature") +
                                 std::format(";type={},identifier={}", func_sig.type.to_str(), func_sig.identifier)};
        elements.emplace_back("FunctionSignature", _nest_level);
        _NestGuard guard{_nest_level};
        std::ranges::for_each(func_sig.params, [this](const auto& typed_ident) { typed_ident->accept(*this); });
    }

    void visit(const ForLoop& for_loop) override {
        elements.emplace_back(_get_name_position_str(for_loop, "ForLoop"), _nest_level);
        _NestGuard guard{_nest_level};
        for_loop.var_declaration->accept(*this);
        for_loop.condition->accept(*this);
        for_loop.loop_update->accept(*this);
        for_loop.body->accept(*this);
    }

    void visit(const BinaryExpression& binary_expr) override {
        elements.emplace_back(_get_name_position_str(binary_expr, expr_kind_to_str(binary_expr.kind)), _nest_level);
        _NestGuard guard{_nest_level};
        binary_expr.left->accept(*this);
        binary_expr.right->accept(*this);
    }

    void visit(const UnaryExpression& unary_expr) override {
        elements.emplace_back(_get_name_position_str(unary_expr, expr_kind_to_str(unary_expr.kind)), _nest_level);
        _NestGuard guard{_nest_level};
        unary_expr.expr->accept(*this);
    }

    void visit(const FunctionCall& func_call_expr) override {
        elements.emplace_back(_get_name_position_str(func_call_expr, expr_kind_to_str(func_call_expr.kind)),
                              _nest_level);
        _NestGuard guard{_nest_level};
        func_call_expr.callee->accept(*this);
        std::ranges::for_each(func_call_expr.argument_list, [this](const auto& arg) { arg->accept(*this); });
    }

    void visit(const BindFront& bind_front_expr) override {
        elements.emplace_back(_get_name_position_str(bind_front_expr, expr_kind_to_str(bind_front_expr.kind)),
                              _nest_level);
        _NestGuard guard{_nest_level};
        std::ranges::for_each(bind_front_expr.argument_list, [this](const auto& arg) { arg->accept(*this); });
        bind_front_expr.target->accept(*this);
    }

    void visit(const ParenExpression& paren_expr) override {
        elements.emplace_back(_get_name_position_str(paren_expr, expr_kind_to_str(paren_expr.kind)), _nest_level);
        _NestGuard guard{_nest_level};
        paren_expr.expr->accept(*this);
    }

    void visit(const TypeCastExpression& type_cast_expr) override {
        std::string type_cast_str{_get_name_position_str(type_cast_expr, expr_kind_to_str(type_cast_expr.kind)) +
                                  std::format(";to_type={}", type_cast_expr.target_type.to_str())};
        elements.emplace_back(type_cast_str, _nest_level);
        _NestGuard guard{_nest_level};
        type_cast_expr.expr->accept(*this);
    }

    void visit(const Identifier& identifier) override {
        std::string ident_str{_get_name_position_str(identifier, expr_kind_to_str(identifier.kind)) +
                              std::format(";name={}", identifier.name)};
        elements.emplace_back(ident_str, _nest_level);
    }
    void visit(const LiteralInt& literal_int) override {
        std::string lit_int_str{_get_name_position_str(literal_int, expr_kind_to_str(literal_int.kind) + "Int") +
                                std::format(";value={}", literal_int.value)};
        elements.emplace_back(lit_int_str, _nest_level);
    }
    void visit(const LiteralFloat& literal_float) override {
        std::string lit_flt_str{_get_name_position_str(literal_float, expr_kind_to_str(literal_float.kind) + "Float") +
                                std::format(";value={}", literal_float.value)};

        elements.emplace_back(lit_flt_str, _nest_level);
    }
    void visit(const LiteralString& literal_string) override {
        elements.emplace_back("LiteralString", _nest_level);
        std::string lit_string_str{
            _get_name_position_str(literal_string, expr_kind_to_str(literal_string.kind) + "String") +
            std::format(R"(;value="{}")", literal_string.value)};

        elements.emplace_back(lit_string_str, _nest_level);
    }
    void visit(const LiteralBool& literal_bool) override {
        std::string lit_bool_str{_get_name_position_str(literal_bool, expr_kind_to_str(literal_bool.kind) + "Bool") +
                                 std::format(";value={}", literal_bool.value ? "true" : "false")};
        elements.emplace_back(lit_bool_str, _nest_level);
    }

    void visit(const TypedIdentifier& typed_ident) override {
        std::string typed_ident_str{_get_name_position_str(typed_ident, "TypedIdentifier") +
                                    std::format(";name={},type={}", typed_ident.name, typed_ident.type.to_str())};
        elements.emplace_back("TypedIdentifier", _nest_level);
    }

    // vector of (std::string element, int nest_level)
    // element = "Elem;pos;additional_info" eg. "LiteralInt;[1:1];value=4"
    // sequence in vector reflects order of elements in program
    // Example:
    // program:
    // {
    //     let inc_42: function<mut int:none> = (42) >> inc_by;
    //     return inc_42
    // }
    // printed as:
    // (note that position is just default - mock lexer sets default value, elements without <0x56f7473e83f0>
    //  are not program elements, but
    // are used for readablility purpose)
    //
    //  [Program] <0x56f7473c3700> at: [1:1]
    //   |_[CodeBlock ] <0x56f7473ca180> at: [1:1]
    //     |_[VariableDeclaration] <0x56f7473e83f0> at: [1:1]
    //       |_[TypedIdentifier ] <0x56f7473e1030> at: [1:1]  identifier:inc_42, type:function<mut int:none>
    //       |_[BindFront] <0x56f7473e03d0> at: [1:1]
    //         |_Arguments:
    //           |_[LiteralInt] <0x56f7473e07c0> at: [1:1] type:int, value:42
    //         |_Bind Target:
    //           |_[Identifier] <0x56f7473e4f20> at: [1:1] name:inc_by
    //     |_[ReturnStatement ] <0x56f7473c32a0> at: [1:1]
    //       |_[Identifier] <0x56f7473e04f0> at: [1:1] name:inc_42
    //
    // will be represented as:
    // [
    //  ("Program;[1:1]", 0), ("VariableDeclaration;[1:1]", 1),
    //  ("TypedIdentifier;[1:1];name=inc_42,type:function<mut int:none>", 2), ("BindFront;[1:1]", 2),
    //  ("LiteralInt;[1:1];type=int,value=42",  3), ("Identifier;[1:1];name=inc_by", 3),
    //  ("ReturnStatement;[1:1]", 1), ("Identifier;[1:1];name=inc_42", 2)
    // ]
    std::vector<std::pair<std::string, int>> elements = {};

   private:
    int _nest_level = 0;
    std::string _get_name_position_str(const Node& node, std::string element_name) {
        return std::format("{};{}", element_name, node.position.get_position_str());
    }
    struct _NestGuard {
        int& level;
        explicit _NestGuard(int& level) : level{level} {
            ++level;
        }
        ~_NestGuard() {
            --level;
        }
    };
};

BOOST_AUTO_TEST_CASE(test_constructor) {
    auto lexer = std::make_unique<MockLexer>();
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 0);
}

// continue;
BOOST_AUTO_TEST_CASE(test_continue) {
    std::vector<Token> tokens = {
        Token{TokenType::T_CONTINUE, Position()},
        Token(TokenType::T_SEMICOLON, Position()),
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 1);
    Printer printer{};
    program->accept(printer);
}

// break;
BOOST_AUTO_TEST_CASE(test_break) {
    std::vector<Token> tokens = {
        Token{TokenType::T_BREAK, Position()},
        Token(TokenType::T_SEMICOLON, Position()),
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 1);
    Printer printer{};
    program->accept(printer);
}

// continue;
// break;
BOOST_AUTO_TEST_CASE(test_break_cont) {
    std::vector<Token> tokens = {
        Token{TokenType::T_CONTINUE, Position()},
        Token(TokenType::T_SEMICOLON, Position()),
        Token{TokenType::T_BREAK, Position()},
        Token(TokenType::T_SEMICOLON, Position()),
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 2);
    Printer printer{};
    program->accept(printer);
}

// return 4;
BOOST_AUTO_TEST_CASE(test_return_literal_int) {
    std::vector<Token> tokens = {
        Token{TokenType::T_RETURN, Position()},
        Token(TokenType::T_LITERAL_INT, Position(), 4),
        Token(TokenType::T_SEMICOLON, Position()),
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 1);
    Printer printer{};
    program->accept(printer);
}

// return (4.5);
BOOST_AUTO_TEST_CASE(test_return_paren) {
    std::vector<Token> tokens = {
        Token{TokenType::T_RETURN, Position()},
        Token{TokenType::T_L_PAREN, Position()},
        Token(TokenType::T_LITERAL_FLOAT, Position(), 4.5),
        Token{TokenType::T_R_PAREN, Position()},
        Token(TokenType::T_SEMICOLON, Position()),
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 1);
    Printer printer{};
    program->accept(printer);
}

// return (3 * 5) / divisor;
BOOST_AUTO_TEST_CASE(test_return_paren_multiplication) {
    std::vector<Token> tokens = {
        Token{TokenType::T_RETURN, Position()},         Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 3}, Token{TokenType::T_MULTIPLY, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 5}, Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_DIVIDE, Position()},         Token{TokenType::T_IDENTIFIER, Position(), "divisor"},
        Token{TokenType::T_SEMICOLON, Position()},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 1);
    Printer printer{};
    program->accept(printer);
}
// let is_foo: bool = (foo())("Hello World")
BOOST_AUTO_TEST_CASE(test_variable_declaration_with_function_call) {
    std::vector<Token> tokens = {
        Token{TokenType::T_LET, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "is_foo"},
        Token{TokenType::T_COLON, Position()},
        Token{TokenType::T_BOOL, Position()},
        Token{TokenType::T_ASSIGN, Position()},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "foo"},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_LITERAL_STRING, Position(), "Hello World"},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_SEMICOLON, Position()},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    Printer printer{};
    program->accept(printer);
}

// {
//  let inc_42: function<mut int: none> = (42) >> inc_by;
//  return inc_42;
// }
BOOST_AUTO_TEST_CASE(test_parse_code_block) {
    std::vector<Token> tokens = {
        Token{TokenType::T_L_BRACE, Position()},
        Token{TokenType::T_LET, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "inc_42"},
        Token{TokenType::T_COLON, Position()},
        Token{TokenType::T_FUNCTION, Position()},
        Token{TokenType::T_LESS, Position()},
        Token{TokenType::T_MUT, Position()},
        Token{TokenType::T_INT, Position()},
        Token{TokenType::T_COLON, Position()},
        Token{TokenType::T_NONE, Position()},
        Token{TokenType::T_GREATER, Position()},
        Token{TokenType::T_ASSIGN, Position()},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 42},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_BIND_FRONT, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "inc_by"},
        Token{TokenType::T_SEMICOLON, Position()},
        Token{TokenType::T_RETURN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "inc_42"},
        Token{TokenType::T_SEMICOLON, Position()},
        Token{TokenType::T_R_BRACE, Position()},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();
    BOOST_CHECK_EQUAL(program->statements.size(), 1);
    Printer printer{};
    program->accept(printer);
}
// if (x < 4){
//     return x + 2;
// } else if (x > 5) {
//     return x - 3;
// } else {
//     return x * 2;
// }
BOOST_AUTO_TEST_CASE(test_if_else_statement) {
    std::vector<Token> tokens = {
        Token{TokenType::T_IF, Position()},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_LESS, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 4},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_L_BRACE, Position()},
        Token{TokenType::T_RETURN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_PLUS, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 2},
        Token{TokenType::T_SEMICOLON, Position()},
        Token{TokenType::T_R_BRACE, Position()},
        Token{TokenType::T_ELSE, Position()},
        Token{TokenType::T_IF, Position()},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_GREATER, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 5},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_L_BRACE, Position()},
        Token{TokenType::T_RETURN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_MINUS, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 3},
        Token{TokenType::T_SEMICOLON, Position()},
        Token{TokenType::T_R_BRACE, Position()},
        Token{TokenType::T_ELSE, Position()},
        Token{TokenType::T_L_BRACE, Position()},
        Token{TokenType::T_RETURN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_MULTIPLY, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 2},
        Token{TokenType::T_SEMICOLON, Position()},
        Token{TokenType::T_R_BRACE, Position()},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    Printer printer{};
    program->accept(printer);
}

// some_num = (4.5 + 8 as float) * 2.5 + sqrt(3) * 1.5 / 9 as float;
BOOST_AUTO_TEST_CASE(test_assignment_with_complex_expression_and_cast) {
    std::vector<Token> tokens = {
        Token{TokenType::T_IDENTIFIER, Position(), "some_num"},
        Token{TokenType::T_ASSIGN, Position()},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_LITERAL_FLOAT, Position(), 4.5},
        Token{TokenType::T_PLUS, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 8},
        Token{TokenType::T_AS, Position()},
        Token{TokenType::T_FLOAT, Position()},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_MULTIPLY, Position()},
        Token{TokenType::T_LITERAL_FLOAT, Position(), 2.5},
        Token{TokenType::T_PLUS, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "sqrt"},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 3},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_MULTIPLY, Position()},
        Token{TokenType::T_LITERAL_FLOAT, Position(), 1.5},
        Token{TokenType::T_DIVIDE, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 9},
        Token{TokenType::T_AS, Position()},
        Token{TokenType::T_FLOAT, Position()},
        Token{TokenType::T_SEMICOLON, Position()},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    Printer printer{};
    program->accept(printer);
}

// (is_primary(x) or x <= 2) and x > 0 or "hello" + " world" == "hello world";

BOOST_AUTO_TEST_CASE(test_complex_logical_and_arithmetic_expression) {
    std::vector<Token> tokens = {
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "is_primary"},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_OR, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_LESS_EQUAL, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 2},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_AND, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "x"},
        Token{TokenType::T_GREATER, Position()},
        Token{TokenType::T_LITERAL_INT, Position(), 0},
        Token{TokenType::T_OR, Position()},
        Token{TokenType::T_LITERAL_STRING, Position(), "hello"},
        Token{TokenType::T_PLUS, Position()},
        Token{TokenType::T_LITERAL_STRING, Position(), " world"},
        Token{TokenType::T_EQUAL, Position()},
        Token{TokenType::T_LITERAL_STRING, Position(), "hello world"},
        Token{TokenType::T_SEMICOLON, Position()},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    Printer printer{};
    program->accept(printer);
}

// let hello_world: function<none:string> = ("Hello", "world") >> space_concat;
BOOST_AUTO_TEST_CASE(test_variable_declaration_with_bind_front) {
    std::vector<Token> tokens = {
        Token{TokenType::T_LET, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "hello_world"},
        Token{TokenType::T_COLON, Position()},
        Token{TokenType::T_FUNCTION, Position()},
        Token{TokenType::T_LESS, Position()},
        Token{TokenType::T_NONE, Position()},
        Token{TokenType::T_COLON, Position()},
        Token{TokenType::T_STRING, Position()},
        Token{TokenType::T_GREATER, Position()},
        Token{TokenType::T_ASSIGN, Position()},
        Token{TokenType::T_L_PAREN, Position()},
        Token{TokenType::T_LITERAL_STRING, Position(), "Hello"},
        Token{TokenType::T_COMMA, Position()},
        Token{TokenType::T_LITERAL_STRING, Position(), "world"},
        Token{TokenType::T_R_PAREN, Position()},
        Token{TokenType::T_BIND_FRONT, Position()},
        Token{TokenType::T_IDENTIFIER, Position(), "space_concat"},
        Token{TokenType::T_SEMICOLON, Position()},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    Printer printer{};
    program->accept(printer);
}

// ((4, 2) >> sum_two && square)();
BOOST_AUTO_TEST_CASE(test_function_composition_with_bind_front) {
    std::vector<Token> tokens = {
        Token{TokenType::T_L_PAREN, Position()},                // (
        Token{TokenType::T_L_PAREN, Position()},                // (
        Token{TokenType::T_LITERAL_INT, Position(), 4},         // 4
        Token{TokenType::T_COMMA, Position()},                  // ,
        Token{TokenType::T_LITERAL_INT, Position(), 2},         // 2
        Token{TokenType::T_R_PAREN, Position()},                // )
        Token{TokenType::T_BIND_FRONT, Position()},             // >>
        Token{TokenType::T_IDENTIFIER, Position(), "sum_two"},  // sum_two
        Token{TokenType::T_FUNC_COMPOSITION, Position()},       // &&
        Token{TokenType::T_IDENTIFIER, Position(), "square"},   // square
        Token{TokenType::T_R_PAREN, Position()},                // )
        Token{TokenType::T_L_PAREN, Position()},                // (
        Token{TokenType::T_R_PAREN, Position()},                // )
        Token{TokenType::T_SEMICOLON, Position()},              // ;
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    Printer printer{};
    program->accept(printer);
}

// def increment(mut a: int) -> none { a = a + 1; }
BOOST_AUTO_TEST_CASE(test_function_definition_with_mutable_argument) {
    std::vector<Token> tokens = {
        Token{TokenType::T_DEF, Position()},                      // def
        Token{TokenType::T_IDENTIFIER, Position(), "increment"},  // increment
        Token{TokenType::T_L_PAREN, Position()},                  // (
        Token{TokenType::T_MUT, Position()},                      // mut
        Token{TokenType::T_IDENTIFIER, Position(), "a"},          // a
        Token{TokenType::T_COLON, Position()},                    // :
        Token{TokenType::T_INT, Position()},                      // int
        Token{TokenType::T_R_PAREN, Position()},                  // )
        Token{TokenType::T_ARROW, Position()},                    // ->
        Token{TokenType::T_NONE, Position()},                     // none
        Token{TokenType::T_L_BRACE, Position()},                  // {
        Token{TokenType::T_IDENTIFIER, Position(), "a"},          // a
        Token{TokenType::T_ASSIGN, Position()},                   // =
        Token{TokenType::T_IDENTIFIER, Position(), "a"},          // a
        Token{TokenType::T_PLUS, Position()},                     // +
        Token{TokenType::T_LITERAL_INT, Position(), 1},           // 1
        Token{TokenType::T_SEMICOLON, Position()},                // ;
        Token{TokenType::T_R_BRACE, Position()},                  // }
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    Printer printer{};
    program->accept(printer);
}

// for (i: int = 0; i < 10; i = i + 1) {
//     print(i);
// }
BOOST_AUTO_TEST_CASE(test_for_loop_with_variable_declaration) {
    std::vector<Token> tokens = {
        Token{TokenType::T_FOR, Position()},                  // for
        Token{TokenType::T_L_PAREN, Position()},              // (
        Token{TokenType::T_IDENTIFIER, Position(), "i"},      // i
        Token{TokenType::T_COLON, Position()},                // :
        Token{TokenType::T_INT, Position()},                  // int
        Token{TokenType::T_ASSIGN, Position()},               // =
        Token{TokenType::T_LITERAL_INT, Position(), 0},       // 0
        Token{TokenType::T_SEMICOLON, Position()},            // ;
        Token{TokenType::T_IDENTIFIER, Position(), "i"},      // i
        Token{TokenType::T_LESS, Position()},                 // <
        Token{TokenType::T_LITERAL_INT, Position(), 10},      // 10
        Token{TokenType::T_SEMICOLON, Position()},            // ;
        Token{TokenType::T_IDENTIFIER, Position(), "i"},      // i
        Token{TokenType::T_ASSIGN, Position()},               // =
        Token{TokenType::T_IDENTIFIER, Position(), "i"},      // i
        Token{TokenType::T_PLUS, Position()},                 // +
        Token{TokenType::T_LITERAL_INT, Position(), 1},       // 1
        Token{TokenType::T_R_PAREN, Position()},              // )
        Token{TokenType::T_L_BRACE, Position()},              // {
        Token{TokenType::T_IDENTIFIER, Position(), "print"},  // print
        Token{TokenType::T_L_PAREN, Position()},              // (
        Token{TokenType::T_IDENTIFIER, Position(), "i"},      // i
        Token{TokenType::T_R_PAREN, Position()},              // )
        Token{TokenType::T_SEMICOLON, Position()},            // ;
        Token{TokenType::T_R_BRACE, Position()},              // }
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    // Użycie Printer do wypisania struktury AST
    Printer printer{};
    program->accept(printer);

    TestVisitor t_visit{};
    program->accept(t_visit);
    std::ranges::for_each(t_visit.elements, [](auto& element) { std::cout << element << " "; });
    std::cout << std::endl;
}

// return -10;
BOOST_AUTO_TEST_CASE(test_return_negative_literal_int) {
    std::vector<Token> tokens = {
        Token{TokenType::T_RETURN, Position()},           // return
        Token{TokenType::T_MINUS, Position()},            // -
        Token{TokenType::T_LITERAL_INT, Position(), 10},  // 10
        Token{TokenType::T_SEMICOLON, Position()},        // ;
    };

    std::vector<std::pair<std::string, int>> expected_elements = {
        {"Program;[1:1]", 0},
        {"ReturnStatement;[1:1]", 1},
        {"UnaryMinus;[1:1]", 2},
        {"LiteralInt;[1:1];value=10", 3},
    };

    auto lexer = std::make_unique<MockLexer>(tokens);
    Parser parser{std::move(lexer)};
    auto program = parser.parse_program();

    BOOST_CHECK_EQUAL(program->statements.size(), 1);

    Printer printer{};
    program->accept(printer);

    TestVisitor t_visit{};
    program->accept(t_visit);
    BOOST_CHECK(expected_elements == t_visit.elements);
}

BOOST_AUTO_TEST_CASE(test_fail) {
    BOOST_CHECK_EQUAL(1, 0);
}
