#include "expression.hpp"

/* -----------------------------------------------------------------------------*
 *                               BASE - EXPRESSION                              *
 *------------------------------------------------------------------------------*/
Expression::Expression(const Position& position, ExpressionKind kind) : Node{position}, kind{kind} {}

/* -----------------------------------------------------------------------------*
 *                               BINARY_EXPRESSION                              *
 *------------------------------------------------------------------------------*/
BinaryExpression::BinaryExpression(const Position& position, ExpressionKind kind, up_expression left,
                                   up_expression right)
    : Expression{position, kind}, left{std::move(left)}, right{std::move(right)} {}

void BinaryExpression::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

const std::unordered_set<ExpressionKind> BinaryExpression::binary_kinds = {
    ExpressionKind::LOGICAL_OR,
    ExpressionKind::LOGICAL_AND,
    ExpressionKind::EQUAL,
    ExpressionKind::NOT_EQUAL,
    ExpressionKind::GREATER,
    ExpressionKind::GREATER_EQUAL,
    ExpressionKind::LESS,
    ExpressionKind::LESS_EQUAL,
    ExpressionKind::ADDITION,
    ExpressionKind::SUBTRACTION,
    ExpressionKind::MULTIPICATION,
    ExpressionKind::DIVISION,
    ExpressionKind::FUNCTION_COMPOSITION,
};

std::unique_ptr<BinaryExpression> BinaryExpression::create(const Position& position, ExpressionKind kind,
                                                           up_expression left, up_expression right) {
    if (not(binary_kinds.contains(kind) and left and right))
        throw std::logic_error("binary expr initialization");  // TODO: replace with custom exception
    return std::make_unique<BinaryExpression>(position, kind, std::move(left), std::move(right));
}

/* -----------------------------------------------------------------------------*
 *                               UNARY_EXPRESSION                               *
 *------------------------------------------------------------------------------*/
UnaryExpression::UnaryExpression(const Position& position, ExpressionKind kind, up_expression expr)
    : Expression{position, kind}, expr{std::move(expr)} {}

void UnaryExpression::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

const std::unordered_set<ExpressionKind> UnaryExpression::unary_kinds = {
    ExpressionKind::LOGICAL_NOT,
    ExpressionKind::UNARY_MINUS,
};

std::unique_ptr<UnaryExpression> UnaryExpression::create(const Position& position, ExpressionKind kind,
                                                         up_expression expr) {
    if (not unary_kinds.contains(kind))
        throw std::logic_error("invalid expr kind");  // TODO: replace with custom exception
    return std::make_unique<UnaryExpression>(position, kind, std::move(expr));
}

/* -----------------------------------------------------------------------------*
 *                          FUNCTION_CALL AND BIND_FRONT                        *
 *------------------------------------------------------------------------------*/
FunctionCall::FunctionCall(const Position& position, up_expression callee, up_expression_vec argument_list)
    : Expression{position, ExpressionKind::FUNCTION_CALL},
      callee{std::move(callee)},
      argument_list{std::move(argument_list)} {}

void FunctionCall::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

BindFront::BindFront(const Position& position, up_expression_vec argument_list, up_expression target)
    : Expression{position, ExpressionKind::BIND_FRONT},
      argument_list{std::move(argument_list)},
      target{std::move(target)} {}

void BindFront::accept(Visitor& visitor) const {
    visitor.visit(*this);
}
/* -----------------------------------------------------------------------------*
 *                           PARENTHESIZED_EXPRESSION                           *
 *------------------------------------------------------------------------------*/
ParenExpression::ParenExpression(const Position& position, up_expression expr)
    : Expression{position, ExpressionKind::PARENTHESIZED}, expr{std::move(expr)} {}

void ParenExpression::accept(Visitor& visitor) const {
    visitor.visit(*this);
}
/* -----------------------------------------------------------------------------*
 *                          TYPE_CAST_EXPRESSION                          *
 *------------------------------------------------------------------------------*/
TypeCastExpression::TypeCastExpression(const Position& position, up_expression expr, Type target_type)
    : Expression{position, ExpressionKind::TYPE_CAST}, expr{std::move(expr)}, target_type{target_type} {}

void TypeCastExpression::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

/* -----------------------------------------------------------------------------*
 *                                 IDENTIFIER                                   *
 *------------------------------------------------------------------------------*/
Identifier::Identifier(const Position& position, std::string name)
    : Expression{position, ExpressionKind::IDENTIFIER}, name{name} {}

void Identifier::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

/* -----------------------------------------------------------------------------*
 *                                  LITERALS                                    *
 *------------------------------------------------------------------------------*/
LiteralInt::LiteralInt(const Position& position, int value)
    : Expression{position, ExpressionKind::LITERAL}, value{value} {}

void LiteralInt::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

LiteralFloat::LiteralFloat(const Position& position, double value)
    : Expression{position, ExpressionKind::LITERAL}, value{value} {}

void LiteralFloat::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

LiteralString::LiteralString(const Position& position, std::string value)
    : Expression{position, ExpressionKind::LITERAL}, value{value} {}

void LiteralString::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

LiteralBool::LiteralBool(const Position& position, bool value)
    : Expression{position, ExpressionKind::LITERAL}, value{value} {}

void LiteralBool::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

/* -----------------------------------------------------------------------------*
 *                               EXPRESSION_KIND                                *
 *------------------------------------------------------------------------------*/
std::string expression_kind_to_str(const ExpressionKind& kind) {
    switch (kind) {
        case ExpressionKind::LOGICAL_OR:
            return "LogicalOr";
        case ExpressionKind::LOGICAL_AND:
            return "LogicalAnd";
        case ExpressionKind::EQUAL:
            return "Equal";
        case ExpressionKind::NOT_EQUAL:
            return "NotEqual";
        case ExpressionKind::GREATER:
            return "Greater";
        case ExpressionKind::GREATER_EQUAL:
            return "GreaterEqual";
        case ExpressionKind::LESS:
            return "Less";
        case ExpressionKind::LESS_EQUAL:
            return "LessEqual";
        case ExpressionKind::ADDITION:
            return "Addition";
        case ExpressionKind::SUBTRACTION:
            return "Subtraction";
        case ExpressionKind::MULTIPICATION:
            return "Multiplication";
        case ExpressionKind::DIVISION:
            return "Division";
        case ExpressionKind::FUNCTION_COMPOSITION:
            return "FunctionComposition";
        case ExpressionKind::BIND_FRONT:
            return "BindFront";
        case ExpressionKind::FUNCTION_CALL:
            return "FunctionCall";
        case ExpressionKind::TYPE_CAST:
            return "TypeCast";
        case ExpressionKind::LOGICAL_NOT:
            return "LogicalNot";
        case ExpressionKind::UNARY_MINUS:
            return "UnaryMinus";
        case ExpressionKind::PARENTHESIZED:
            return "Parenthesized";
        case ExpressionKind::IDENTIFIER:
            return "Identifier";
        case ExpressionKind::LITERAL:
            return "Literal";
    }
}
