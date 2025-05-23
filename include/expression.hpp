#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP
#include <memory>
#include <unordered_set>

#include "node.hpp"
#include "type.hpp"

enum class ExprKind {
    LOGICAL_OR,
    LOGICAL_AND,
    EQUAL,
    NOT_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    ADDITION,
    SUBTRACTION,
    MULTIPICATION,
    DIVISION,
    FUNCTION_COMPOSITION,

    BIND_FRONT,
    FUNCTION_CALL,

    TYPE_CAST,
    LOGICAL_NOT,
    UNARY_MINUS,

    IDENTIFIER,
    LITERAL,
};

std::string expr_kind_to_str(const ExprKind& expr_kind);

struct Expression : Node {
    explicit Expression(const Position& position, ExprKind expr_kind);
    ExprKind kind;
};

using up_expression = std::unique_ptr<Expression>;
using up_expression_vec = std::vector<up_expression>;

struct BinaryExpression : Expression {
    explicit BinaryExpression(ExprKind kind, up_expression left, up_expression right);
    up_expression left;
    up_expression right;

    void accept(Visitor& visitor) const override;

    static const std::unordered_set<ExprKind> binary_kinds;
    static std::unique_ptr<BinaryExpression> create(ExprKind kind, up_expression left, up_expression right);
};

struct UnaryExpression : Expression {
    explicit UnaryExpression(const Position& position, ExprKind kind, up_expression expr);
    up_expression expr;

    void accept(Visitor& visitor) const override;

    static const std::unordered_set<ExprKind> unary_kinds;
    static std::unique_ptr<UnaryExpression> create(const Position& position, ExprKind kind, up_expression expr);
};

struct FunctionCall : Expression {
    explicit FunctionCall(up_expression callee, up_expression_vec argument_list);
    up_expression callee;
    up_expression_vec argument_list;

    void accept(Visitor& visitor) const override;
};

struct BindFront : Expression {
    explicit BindFront(const Position& position, up_expression_vec argument_list, up_expression target);
    up_expression_vec argument_list;
    up_expression target;

    void accept(Visitor& visitor) const override;
};

struct TypeCastExpression : Expression {
    explicit TypeCastExpression(up_expression expr, Type target_type);
    up_expression expr;
    Type target_type;

    void accept(Visitor& visitor) const override;
};

struct Identifier : Expression {
    explicit Identifier(const Position& position, std::string name);
    std::string name;

    void accept(Visitor& visitor) const override;
};

struct LiteralInt : Expression {
    explicit LiteralInt(const Position& position, int value);
    Type type;
    int value;

    void accept(Visitor& visitor) const override;
};

struct LiteralFloat : Expression {
    explicit LiteralFloat(const Position& position, double value);
    Type type = Type(TypeKind::INT);
    double value;

    void accept(Visitor& visitor) const override;
};

struct LiteralString : Expression {
    explicit LiteralString(const Position& position, std::string value);
    Type type = Type(TypeKind::STRING);
    std::string value;

    void accept(Visitor& visitor) const override;
};

struct LiteralBool : Expression {
    explicit LiteralBool(const Position& position, bool value);
    Type type = Type(TypeKind::BOOL);
    bool value;

    void accept(Visitor& visitor) const override;
};

#endif  // EXPRESSION_HPP