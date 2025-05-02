#ifndef PRINTER_HPP
#define PRINTER_HPP
#include <string>

#include "visitor.hpp"

struct Expression;

class Printer : public Visitor {
   public:
    void visit(const Program& program) override;
    void visit(const ContinueStatement& continue_stmnt) override;
    void visit(const BreakStatement& break_stmnt) override;

    void visit(const BinaryExpression& binary_expr) override;
    void visit(const UnaryExpression& unary_expr) override;
    void visit(const FunctionCall& func_call_expr) override;
    void visit(const BindFront& bind_front_expr) override;
    void visit(const ParenExpression& paren_expr) override;
    void visit(const TypeCastExpression& type_cast_expr) override;
    void visit(const Identifier& identifier) override;
    void visit(const LiteralInt& literal_int) override;
    void visit(const LiteralFloat& literal_float) override;
    void visit(const LiteralString& lieteral_string) override;
    void visit(const LiteralBool& literal_bool) override;

    void visit(const TypedIdentifier& typed_ident) override;

    ~Printer() = default;

   private:
    int _indent_level = 0;
    void _print_indent() const;
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
