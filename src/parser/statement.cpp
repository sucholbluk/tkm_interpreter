#include "statement.hpp"

void ContinueStatement::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void BreakStatement::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

ReturnStatement::ReturnStatement(const Position& position, up_expression expression)
    : Statement{position}, expression{std::move(expression)} {}

void ReturnStatement::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

VariableDeclaration::VariableDeclaration(const Position& position, up_typed_identifier typed_identifier,
                                         up_expression expression)
    : Statement{position}, typed_identifier{std::move(typed_identifier)}, assigned_expression{std::move(expression)} {}

void VariableDeclaration::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

CodeBlock::CodeBlock(const Position& position, up_statement_vec statements)
    : Statement{position}, statements{std::move(statements)} {}

void CodeBlock::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

IfStatement::IfStatement(const Position& position, up_expression condition, up_statement body, up_else_if_vec else_ifs,
                         up_statement else_body)
    : Statement{position},
      condition{std::move(condition)},
      body{std::move(body)},
      else_ifs{std::move(else_ifs)},
      else_body{std::move(else_body)} {}

void IfStatement::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

ElseIf::ElseIf(const Position& position, up_expression condition, up_statement body)
    : Statement{position}, condition{std::move(condition)}, body{std::move(body)} {}

void ElseIf::accept(Visitor& visitor) const {
    visitor.visit(*this);
}