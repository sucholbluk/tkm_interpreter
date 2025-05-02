#ifndef VISITIOR_HPP
#define VISITIOR_HPP

struct Program;
struct ContinueStatement;
struct BreakStatement;
struct ReturnStatement;

struct BinaryExpression;
struct UnaryExpression;
struct FunctionCall;
struct BindFront;
struct ParenExpression;
struct Identifier;
struct LiteralInt;
struct LiteralFloat;
struct LiteralString;
struct LiteralBool;
struct TypedIdentifier;
struct TypeCastExpression;
struct VariableDeclarationStatement;

class Visitor {
   public:
    virtual ~Visitor() = default;

    virtual void visit(const Program& program) = 0;
    virtual void visit(const ContinueStatement& continue_stmnt) = 0;
    virtual void visit(const BreakStatement& break_stmnt) = 0;
    virtual void visit(const ReturnStatement& reaturn_stmnt) = 0;
    virtual void visit(const VariableDeclarationStatement& var_decl) = 0;

    virtual void visit(const BinaryExpression& binary_expr) = 0;
    virtual void visit(const UnaryExpression& unary_expr) = 0;
    virtual void visit(const FunctionCall& func_call_expr) = 0;
    virtual void visit(const BindFront& bind_front_expr) = 0;
    virtual void visit(const ParenExpression& paren_expr) = 0;
    virtual void visit(const TypeCastExpression& type_cast_expr) = 0;
    virtual void visit(const Identifier& identifier) = 0;
    virtual void visit(const LiteralInt& literal_int) = 0;
    virtual void visit(const LiteralFloat& literal_float) = 0;
    virtual void visit(const LiteralString& lieteral_string) = 0;
    virtual void visit(const LiteralBool& literal_bool) = 0;

    virtual void visit(const TypedIdentifier& typed_ident) = 0;
};

#endif  // VISITOR_HPP