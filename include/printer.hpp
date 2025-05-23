#ifndef PRINTER_HPP
#define PRINTER_HPP
#include <string>

#include "visitor.hpp"

struct Expression;
/**
 * @ingroup parser
 * @brief Visitor that prints the program tree.
 */
class Printer : public Visitor {
   public:
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

    ~Printer() = default;

   private:
    int _indent_level = 0;
    void _print_indent() const;
    void _print_header(std::string type_str, const Node& node, std::string additional_info = "") const;
    void _print_expression_header(const Expression& expr, std::string type_spec = "",
                                  std::string additional_info = "") const;

    struct _IndentGuard {
        int& level;
        explicit _IndentGuard(int& level) : level{level} {
            ++level;
        }
        ~_IndentGuard() {
            --level;
        }
    };
};

#endif  // PRINTER_HPP
