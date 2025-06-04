#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "builtint_functions.hpp"
#include "call_frame.hpp"
#include "composed_function.hpp"
#include "environment.hpp"
#include "visitor.hpp"

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
    void visit(const FunctionDefinition& func_def) override;
    void visit(const FunctionCall& func_call) override;
    void visit(const Identifier& identifier) override;
    void visit(const LiteralString& literal_string) override;
    void visit(const ReturnStatement& return_stmnt) override;
    void visit(const CodeBlock& code_block) override;
    void visit(const ExpressionStatement& expr_stmnt) override;
    void visit(const LiteralInt& literal_int) override;
    void visit(const LiteralFloat& literal_float) override;
    void visit(const LiteralBool& literal_bool) override;
    void visit(const TypeCastExpression& type_cast_expr) override;
    void visit(const VariableDeclaration& var_decl) override;
    void visit(const AssignStatement& asgn_stmnt) override;
    void visit(const BinaryExpression& binary_expr) override;
    void visit(const UnaryExpression& unary_expr) override;
    void visit(const IfStatement& if_stmnt) override;
    void visit(const ElseIf& else_if) override;
    void visit(const BindFront& bind_front_expr) override;
    void visit(const ContinueStatement& continue_stmnt) override;
    void visit(const BreakStatement& break_stmnt) override;

    void visit(const ForLoop& for_loop) override;

    void visit(const FunctionSignature& func_sig) override{};
    void visit(const TypedIdentifier& typed_ident) override{};

   private:
    opt_vhold_or_val _tmp_result;
    Environment _env;
    bool _is_returning = false;
    bool _condition_met = false;
    bool _inside_loop = false;
    bool _on_continue = false;
    bool _on_break = false;

    void _execute_main();
    void _clear_tmp_result();
    void _handle_function_call_end();
    arg_list _get_arg_list(const up_expression_vec& arguments);
    bool _tmp_result_is_empty() const;
    bool _should_exit_code_block() const;
    void _evaluate_binary_expr(const ExprKind& expr_kind, value left, value right);
    void _evaluate_unary_expr(const ExprKind& expr_kind, value val);
    void _evaluate_condition(const Position& condition_pos);
    void _enter_loop();
    void _exit_loop();

    friend class GlobalFunction;
    friend class ComposedFunction;
    friend class BuiltinFunction;
};

#endif  // INTERPRETER_HPP