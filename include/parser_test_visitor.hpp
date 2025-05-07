#ifndef PARSER_TEST_VISITOR_HPP
#define PARSER_TEST_VISITOR_HPP

#include <format>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "visitor.hpp"

struct ParserTestVisitor : public Visitor {
    void visit(const Program& program) override;
    void visit(const ContinueStatement& continue_stmnt) override;
    void visit(const BreakStatement& break_stmnt) override;
    void visit(const ReturnStatement& return_stmnt) override;
    void visit(const VariableDeclaration& var_decl) override;
    void visit(const CodeBlock& code_block) override;
    void visit(const IfStatement& if_stmnt) override;
    void visit(const ElseIf& else_if) override;
    void visit(const AssignStatement& asgn_stmnt) override;
    void visit(const ExpressionStatement& expr_stmnt) override;
    void visit(const FunctionDefinition& func_def) override;
    void visit(const FunctionSignature& func_sig) override;
    void visit(const ForLoop& for_loop) override;
    void visit(const BinaryExpression& binary_expr) override;
    void visit(const UnaryExpression& unary_expr) override;
    void visit(const FunctionCall& func_call_expr) override;
    void visit(const BindFront& bind_front_expr) override;
    void visit(const TypeCastExpression& type_cast_expr) override;
    void visit(const Identifier& identifier) override;
    void visit(const LiteralInt& literal_int) override;
    void visit(const LiteralFloat& literal_float) override;
    void visit(const LiteralString& literal_string) override;
    void visit(const LiteralBool& literal_bool) override;
    void visit(const TypedIdentifier& typed_ident) override;

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
    std::string _get_name_position_str(const Node& node, std::string element_name) const;
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

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2> pair) {
    os << std::format("({}, {})", pair.first, pair.second);
    return os;
}

#endif  // PARSER_TEST_VISITOR_HPP