#include "expression.hpp"

/* -----------------------------------------------------------------------------*
 *                               BASE - EXPRESSION                              *
 *------------------------------------------------------------------------------*/
Expression::Expression(const Position& position, ExprKind kind) : Node{position}, kind{kind} {}

/* -----------------------------------------------------------------------------*
 *                               BINARY_EXPRESSION                              *
 *------------------------------------------------------------------------------*/
BinaryExpression::BinaryExpression(ExprKind kind, up_expression left, up_expression right)
    : Expression{left->position, kind}, left{std::move(left)}, right{std::move(right)} {}

void BinaryExpression::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

const std::unordered_set<ExprKind> BinaryExpression::binary_kinds = {
    ExprKind::LOGICAL_OR,
    ExprKind::LOGICAL_AND,
    ExprKind::EQUAL,
    ExprKind::NOT_EQUAL,
    ExprKind::GREATER,
    ExprKind::GREATER_EQUAL,
    ExprKind::LESS,
    ExprKind::LESS_EQUAL,
    ExprKind::ADDITION,
    ExprKind::SUBTRACTION,
    ExprKind::MULTIPICATION,
    ExprKind::DIVISION,
    ExprKind::FUNCTION_COMPOSITION,
};

std::unique_ptr<BinaryExpression> BinaryExpression::create(ExprKind kind, up_expression left, up_expression right) {
    if (not(binary_kinds.contains(kind) and left and right))
        throw std::logic_error("binary expr initialization");  // TODO: replace with custom exception
    return std::make_unique<BinaryExpression>(kind, std::move(left), std::move(right));
}

/* -----------------------------------------------------------------------------*
 *                               UNARY_EXPRESSION                               *
 *------------------------------------------------------------------------------*/
UnaryExpression::UnaryExpression(const Position& position, ExprKind kind, up_expression expr)
    : Expression{position, kind}, expr{std::move(expr)} {}

void UnaryExpression::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

const std::unordered_set<ExprKind> UnaryExpression::unary_kinds = {
    ExprKind::LOGICAL_NOT,
    ExprKind::UNARY_MINUS,
};

std::unique_ptr<UnaryExpression> UnaryExpression::create(const Position& position, ExprKind kind, up_expression expr) {
    if (not unary_kinds.contains(kind))
        throw std::logic_error("invalid expr kind");  // TODO: replace with custom exception
    return std::make_unique<UnaryExpression>(position, kind, std::move(expr));
}

/* -----------------------------------------------------------------------------*
 *                          FUNCTION_CALL AND BIND_FRONT                        *
 *------------------------------------------------------------------------------*/
FunctionCall::FunctionCall(up_expression callee, up_expression_vec argument_list)
    : Expression{callee->position, ExprKind::FUNCTION_CALL},
      callee{std::move(callee)},
      argument_list{std::move(argument_list)} {}

void FunctionCall::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

BindFront::BindFront(const Position& position, up_expression_vec argument_list, up_expression target)
    : Expression{position, ExprKind::BIND_FRONT}, argument_list{std::move(argument_list)}, target{std::move(target)} {}

void BindFront::accept(Visitor& visitor) const {
    visitor.visit(*this);
}
/* -----------------------------------------------------------------------------*
 *                           PARENTHESIZED_EXPRESSION                           *
 *------------------------------------------------------------------------------*/
ParenExpression::ParenExpression(const Position& position, up_expression expr)
    : Expression{position, ExprKind::PARENTHESIZED}, expr{std::move(expr)} {}

void ParenExpression::accept(Visitor& visitor) const {
    visitor.visit(*this);
}
/* -----------------------------------------------------------------------------*
 *                          TYPE_CAST_EXPRESSION                          *
 *------------------------------------------------------------------------------*/
TypeCastExpression::TypeCastExpression(up_expression expr, Type target_type)
    : Expression{expr->position, ExprKind::TYPE_CAST}, expr{std::move(expr)}, target_type{target_type} {}

void TypeCastExpression::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

/* -----------------------------------------------------------------------------*
 *                                 IDENTIFIER                                   *
 *------------------------------------------------------------------------------*/
Identifier::Identifier(const Position& position, std::string name)
    : Expression{position, ExprKind::IDENTIFIER}, name{name} {}

void Identifier::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

/* -----------------------------------------------------------------------------*
 *                                  LITERALS                                    *
 *------------------------------------------------------------------------------*/
LiteralInt::LiteralInt(const Position& position, int value)
    : Expression{position, ExprKind::LITERAL}, type{TypeKind::INT}, value{value} {}

void LiteralInt::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

LiteralFloat::LiteralFloat(const Position& position, double value)
    : Expression{position, ExprKind::LITERAL}, type{TypeKind::FLOAT}, value{value} {}

void LiteralFloat::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

LiteralString::LiteralString(const Position& position, std::string value)
    : Expression{position, ExprKind::LITERAL}, type{TypeKind::STRING}, value{value} {}

void LiteralString::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

LiteralBool::LiteralBool(const Position& position, bool value)
    : Expression{position, ExprKind::LITERAL}, type{TypeKind::BOOL}, value{value} {}

void LiteralBool::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

/* -----------------------------------------------------------------------------*
 *                               EXPRESSION_KIND                                *
 *------------------------------------------------------------------------------*/
std::string expression_kind_to_str(const ExprKind& kind) {
    switch (kind) {
        case ExprKind::LOGICAL_OR:
            return "LogicalOr";
        case ExprKind::LOGICAL_AND:
            return "LogicalAnd";
        case ExprKind::EQUAL:
            return "Equal";
        case ExprKind::NOT_EQUAL:
            return "NotEqual";
        case ExprKind::GREATER:
            return "Greater";
        case ExprKind::GREATER_EQUAL:
            return "GreaterEqual";
        case ExprKind::LESS:
            return "Less";
        case ExprKind::LESS_EQUAL:
            return "LessEqual";
        case ExprKind::ADDITION:
            return "Addition";
        case ExprKind::SUBTRACTION:
            return "Subtraction";
        case ExprKind::MULTIPICATION:
            return "Multiplication";
        case ExprKind::DIVISION:
            return "Division";
        case ExprKind::FUNCTION_COMPOSITION:
            return "FunctionComposition";
        case ExprKind::BIND_FRONT:
            return "BindFront";
        case ExprKind::FUNCTION_CALL:
            return "FunctionCall";
        case ExprKind::TYPE_CAST:
            return "TypeCast";
        case ExprKind::LOGICAL_NOT:
            return "LogicalNot";
        case ExprKind::UNARY_MINUS:
            return "UnaryMinus";
        case ExprKind::PARENTHESIZED:
            return "Parenthesized";
        case ExprKind::IDENTIFIER:
            return "Identifier";
        case ExprKind::LITERAL:
            return "Literal";
    }
}
