#include "statement.hpp"

ReturnStatement::ReturnStatement(const Position& position, up_expression expression)
    : Statement{position}, expression{std::move(expression)} {}

VariableDeclarationStatement::VariableDeclarationStatement(const Position& position, up_typed_identifier typed_identifier,
                                                           up_expression expression)
    : Statement{position}, typed_identifier{std::move(typed_identifier)}, assigned_expression{std::move(expression)} {}