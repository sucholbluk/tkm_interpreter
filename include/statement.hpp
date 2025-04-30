#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include <memory>

#include "expression.hpp"
#include "node.hpp"
#include "typed_identifier.hpp"

struct Statement : public Node {
    using Node::Node;
};

using up_statement = std::unique_ptr<Statement>;

struct ContinueStatement : public Statement {
    using Statement::Statement;
};

struct BreakStatement : public Statement {
    using Statement::Statement;
};

struct ReturnStatement : public Statement {
    explicit ReturnStatement(const Position& position, up_expression expression = nullptr);
    up_expression expression;
};

struct VariableDeclarationStatement : public Statement {
    up_typed_identifier typed_identifier;
    up_expression assigned_expression;
    explicit VariableDeclarationStatement(const Position& position, up_typed_identifier typed_identifier,
                                          up_expression assigned_expression);
};

#endif  // STATEMENT_HPP