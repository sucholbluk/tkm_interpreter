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
    void accept(Visitor& visitor) const override;
};

struct BreakStatement : public Statement {
    using Statement::Statement;
    void accept(Visitor& visitor) const override;
};

struct ReturnStatement : public Statement {
    explicit ReturnStatement(const Position& position, up_expression expression = nullptr);
    up_expression expression;
    void accept(Visitor& visitor) const override;
};

struct VariableDeclarationStatement : public Statement {
    explicit VariableDeclarationStatement(const Position& position, up_typed_identifier typed_identifier,
                                          up_expression assigned_expression);
    up_typed_identifier typed_identifier;
    up_expression assigned_expression;
    void accept(Visitor& visitor) const override;
};

#endif  // STATEMENT_HPP