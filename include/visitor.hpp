#ifndef VISITIOR_HPP
#define VISITIOR_HPP

struct Node;
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
struct VariableDeclaration;
struct CodeBlock;
struct IfStatement;
struct ElseIf;
struct AssignStatement;
struct ExpressionStatement;
struct FunctionDefinition;
struct FunctionSignature;
struct ForLoop;

class Visitor {
   public:
    virtual ~Visitor() = default;

    virtual void visit(const Program& program) = 0;
    virtual void visit(const ContinueStatement& continue_stmnt) = 0;
    virtual void visit(const BreakStatement& break_stmnt) = 0;
    virtual void visit(const ReturnStatement& reaturn_stmnt) = 0;
    virtual void visit(const VariableDeclaration& var_decl) = 0;
    virtual void visit(const CodeBlock& var_decl) = 0;
    virtual void visit(const IfStatement& if_stmnt) = 0;
    virtual void visit(const ElseIf& else_if) = 0;
    virtual void visit(const AssignStatement& asgn_stmnt) = 0;
    virtual void visit(const ExpressionStatement& expr_stmnt) = 0;
    virtual void visit(const FunctionDefinition& func_def) = 0;
    virtual void visit(const FunctionSignature& func_sig) = 0;
    virtual void visit(const ForLoop& for_loop) = 0;

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