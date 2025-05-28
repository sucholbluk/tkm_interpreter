#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "call_frame.hpp"
#include "environment.hpp"
#include "visitor.hpp"

class PrintFunction;

/**
 * @defgroup interpreter Interpreter
 * @brief Module responsible for code interpretation.
 */

/**
 * @ingroup interpreter
 * @brief Takes program tree and interprets it.
 */
class Interpreter : public Visitor {
   public:
    Interpreter() = default;
    void visit(const Program& program) override;

    void visit(const ContinueStatement& continue_stmnt) override{};
    void visit(const BreakStatement& break_stmnt) override{};
    void visit(const ReturnStatement& return_stmnt) override{};
    void visit(const VariableDeclaration& var_decl) override{};
    void visit(const CodeBlock& code_block) override{};
    void visit(const IfStatement& if_stmnt) override{};
    void visit(const ElseIf& else_if) override{};
    void visit(const AssignStatement& asgn_stmnt) override{};
    void visit(const ExpressionStatement& expr_stmnt) override{};
    void visit(const FunctionDefinition& func_def) override{};
    void visit(const FunctionSignature& func_sig) override{};
    void visit(const ForLoop& for_loop) override{};
    void visit(const BinaryExpression& binary_expr) override{};
    void visit(const UnaryExpression& unary_expr) override{};
    void visit(const FunctionCall& func_call_expr) override{};
    void visit(const BindFront& bind_front_expr) override{};
    void visit(const TypeCastExpression& type_cast_expr) override{};
    void visit(const Identifier& identifier) override{};
    void visit(const LiteralInt& literal_int) override{};
    void visit(const LiteralFloat& literal_float) override{};
    void visit(const LiteralString& literal_string) override{};
    void visit(const LiteralBool& literal_bool) override{};
    void visit(const TypedIdentifier& typed_ident) override{};

    //    private:
    std::optional<value> _temp_result;
    Environment _env;

    void _execute_main();
};

#endif  // INTERPRETER_HPP