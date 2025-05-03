#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include <memory>

#include "expression.hpp"
#include "node.hpp"
#include "typed_identifier.hpp"

struct Statement : public Node {
    using Node::Node;
};

struct CodeBlock;
struct ElseIf;

using up_statement = std::unique_ptr<Statement>;
using up_statement_vec = std::vector<up_statement>;
using up_else_if = std::unique_ptr<ElseIf>;
using up_else_if_vec = std::vector<up_else_if>;

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

struct VariableDeclaration : public Statement {
    explicit VariableDeclaration(const Position& position, up_typed_identifier typed_identifier,
                                 up_expression assigned_expression);
    up_typed_identifier typed_identifier;
    up_expression assigned_expression;
    void accept(Visitor& visitor) const override;
};

struct CodeBlock : public Statement {
    explicit CodeBlock(const Position& position, up_statement_vec statements);
    up_statement_vec statements;
    void accept(Visitor& visitor) const override;
};

struct IfStatement : public Statement {
    explicit IfStatement(const Position& position, up_expression condition, up_statement body, up_else_if_vec else_ifs,
                         up_statement else_body);
    up_expression condition;
    up_statement body;
    up_else_if_vec else_ifs;
    up_statement else_body;
    void accept(Visitor& visitor) const override;
};

struct ElseIf : public Statement {
    explicit ElseIf(const Position& position, up_expression condition, up_statement body);
    up_expression condition;
    up_statement body;
    void accept(Visitor& visitor) const override;
};

#endif  // STATEMENT_HPP