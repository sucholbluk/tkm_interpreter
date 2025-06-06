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
    /**
     * @brief Starts interpretation of the program.
     * @param program Reference to the root Program
     */
    void visit(const Program& program) override;

    /**
     * @brief Registers global function.
     * @param func_def Reference to the function definition
     */
    void visit(const FunctionDefinition& func_def) override;

    /**
     * @brief Executes a function call.
     * @param func_call Reference to the function call
     */
    void visit(const FunctionCall& func_call) override;

    /**
     * @brief Evaluates identifier as variable/function reference
     * @param identifier Reference to the identifier expr.
     *
     * If doesnt no function/variable exists with this identifier throws error.
     */
    void visit(const Identifier& identifier) override;

    /**
     * @brief Handles returning a value from a function.
     * @param return_stmnt Reference to the return statement
     */
    void visit(const ReturnStatement& return_stmnt) override;

    /**
     * @brief Executes a code block by running its statements.
     * @param code_block Reference to the code block
     */
    void visit(const CodeBlock& code_block) override;

    /**
     * @brief Executes an expression statement.
     * @param expr_stmnt Reference to the expression statement
     */
    void visit(const ExpressionStatement& expr_stmnt) override;  // wykonanie wyrazenia wewnatrz

    /**
     * @brief Evaluates a string literal.
     * @param literal_string Reference to the string literal
     */
    void visit(const LiteralString& literal_string) override;

    /**
     * @brief Evaluates an integer literal.
     * @param literal_int Reference to the integer literal
     */
    void visit(const LiteralInt& literal_int) override;

    /**
     * @brief Evaluates a float literal.
     * @param literal_float Reference to the float literal
     */
    void visit(const LiteralFloat& literal_float) override;

    /**
     * @brief Evaluates a boolean literal.
     * @param literal_bool Reference to the boolean literal
     */
    void visit(const LiteralBool& literal_bool) override;

    /**
     * @brief Handles type casting.
     * @param type_cast_expr Reference to the type cast expr
     *
     * throws exception when conversion is not possible
     */
    void visit(const TypeCastExpression& type_cast_expr) override;

    /**
     * @brief Declares a variable if possible.
     * @param var_decl Reference to the variable declaration
     *
     * throws exception if declaration not possible - eg. name conflict
     */
    void visit(const VariableDeclaration& var_decl) override;

    /**
     * @brief Performs assignment if possible.
     * @param asgn_stmnt Reference to the assignment statement
     */
    void visit(const AssignStatement& asgn_stmnt) override;

    /**
     * @brief Evaluates a binary expression.
     * @param binary_expr Reference to the binary expression
     *
     * +,-,*,/, ... also with funciton composition &
     */
    void visit(const BinaryExpression& binary_expr) override;

    /**
     * @brief Evaluates a unary expression.
     * @param unary_expr Reference to the unary expression
     *
     * -, not
     */
    void visit(const UnaryExpression& unary_expr) override;

    /**
     * @brief Interprets if statement
     * @param if_stmnt Reference to the if statement
     *
     */
    void visit(const IfStatement& if_stmnt) override;

    /**
     * @brief Interprets else if
     * @param else_if Reference to the else-if node.
     *
     * Its called from the if_statement's visit
     */
    void visit(const ElseIf& else_if) override;

    /**
     * @brief Evaluates a bind front expression.
     * @param bind_front_expr Reference to the bind front expression node.
     *
     * If correct results in a new callable with bound arguments to the target funciton
     */
    void visit(const BindFront& bind_front_expr) override;

    /**
     * @brief Interprets for loop.
     * @param for_loop Reference to the for loop node.
     */
    void visit(const ForLoop& for_loop) override;

    /**
     * @brief Handles continue statement
     * @param continue_stmnt Reference to the continue statement
     *
     * throws exception if visited outside for loop
     */
    void visit(const ContinueStatement& continue_stmnt) override;

    /**
     * @brief Handles break statement
     * @param break_stmnt Reference to the break statement
     *
     * throws exception if visited outside for loop
     */
    void visit(const BreakStatement& break_stmnt) override;

    void visit(const FunctionSignature& func_sig) override{};
    void visit(const TypedIdentifier& typed_ident) override{};

   private:
    /**
     * @brief Temporary result holder for expression evaluation.
     */
    opt_vhold_or_val _tmp_result;

    /**
     * @brief Interpreter's Environment. Handles function/variables storing and visibility
     */
    Environment _env;

    /**
     * @brief Indicates if on return.
     */
    bool _is_returning = false;

    /**
     * @brief Indicates if a condition was met (for control flow).
     *
     * Used only after _evaluate_condition
     */
    bool _condition_met = false;

    /**
     * @brief Indicates if currently inside a for-loop.
     */
    bool _inside_loop = false;

    /**
     * @brief Indicates if a continue statement was encountered.
     */
    bool _on_continue = false;

    /**
     * @brief Indicates if a break statement was encountered.
     */
    bool _on_break = false;

    /**
     * @brief Calls the main function of the program.
     *
     * artificial call to main function
     */
    void _execute_main();

    /**
     * @brief Clears the temporary result holder.
     */
    void _clear_tmp_result();

    /**
     * @brief Handles cleanup and state reset after a function call ends.
     */
    void _handle_function_call_end();

    /**
     * @brief Evaluates argument expressions as call arguments.
     * @param arguments Vector of unique pointers to argument expressions.
     * @return List of evaluated arguments.
     */
    arg_list _get_arg_list(const up_expression_vec& arguments);

    /**
     * @brief Checks if the temporary result holder is empty.
     * @return True if _tmp_result is empty, false otherwise.
     */
    bool _tmp_result_is_empty() const;

    /**
     * @brief Determines if code block execution should be exited.
     * @return True if execution should exit the current code block.
     *
     * Based on _is_returning, _on_break, _on_continue
     */
    bool _should_exit_code_block() const;

    /**
     * @brief Delegate binary expression evaluation
     * @param expr_kind Kind of binary expression.
     * @param left Left operand value.
     * @param right Right operand value.
     */
    void _evaluate_binary_expr(const ExprKind& expr_kind, value left, value right);

    /**
     * @brief Delegate unary expression evaluation.
     * @param expr_kind Kind of unary expression.
     * @param val Operand value.
     */
    void _evaluate_unary_expr(const ExprKind& expr_kind, value val);

    /**
     * @brief Evaluates a condition based on _tmp_result
     * @param condition_pos Position of the condition in the source code.
     */
    void _evaluate_condition(const Position& condition_pos);

    /**
     * @brief Loop setup.
     *
     * _inside_loop = true, push new scope for loop variable
     */
    void _enter_loop();

    /**
     * @brief Loop cleanup
     *
     * _inside_loop = false, _on_break=false, _on_continue=false, pop scope
     */
    void _exit_loop();

    friend class GlobalFunction;
    friend class ComposedFunction;
    friend class BuiltinFunction;
};

#endif  // INTERPRETER_HPP