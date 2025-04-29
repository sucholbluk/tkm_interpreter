#include "statement.hpp"

ReturnStatement::ReturnStatement(const Position& position, up_expression expression)
    : Statement{position}, expression{std::move(expression)} {}