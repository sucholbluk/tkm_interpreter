#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include <memory>

#include "expression.hpp"
#include "node.hpp"

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

#endif  // STATEMENT_HPP