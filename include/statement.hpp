#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include <memory>

#include "expression.hpp"
#include "node.hpp"
#include "typed_identifier.hpp"
/**
 * @ingroup parser
 * @brief Base class for all statements.
 */
struct Statement : public Node {
    using Node::Node;
};

struct CodeBlock;
struct ElseIf;
struct FunctionSignature;
struct FunctionDefinition;

using up_statement = std::unique_ptr<Statement>;
using up_statement_vec = std::vector<up_statement>;
using up_else_if = std::unique_ptr<ElseIf>;
using up_else_if_vec = std::vector<up_else_if>;
using up_func_sig = std::unique_ptr<FunctionSignature>;
using up_fun_def = std::unique_ptr<FunctionDefinition>;
using up_fun_def_vec = std::vector<up_fun_def>;

/**
 * @ingroup parser
 * @brief Continue statement representation.
 */
struct ContinueStatement : public Statement {
    using Statement::Statement;
    void accept(Visitor& visitor) const override;
};

/**
 * @ingroup parser
 * @brief Break statement representation.
 */
struct BreakStatement : public Statement {
    using Statement::Statement;
    void accept(Visitor& visitor) const override;
};

/**
 * @ingroup parser
 * @brief Return statement representation.
 */
struct ReturnStatement : public Statement {
    explicit ReturnStatement(const Position& position, up_expression expression = nullptr);
    up_expression expression;
    void accept(Visitor& visitor) const override;
};

/**
 * @ingroup parser
 * @brief Variable declaration statement representation.
 */
struct VariableDeclaration : public Statement {
    explicit VariableDeclaration(const Position& position, up_typed_identifier typed_identifier,
                                 up_expression assigned_expression);
    up_typed_identifier typed_identifier;
    up_expression assigned_expression;
    void accept(Visitor& visitor) const override;
};

/**
 * @ingroup parser
 * @brief Code block representation.
 */
struct CodeBlock : public Statement {
    explicit CodeBlock(const Position& position, up_statement_vec statements);
    up_statement_vec statements;
    void accept(Visitor& visitor) const override;
};

/**
 * @ingroup parser
 * @brief If statement representation.
 */
struct IfStatement : public Statement {
    explicit IfStatement(const Position& position, up_expression condition, up_statement body, up_else_if_vec else_ifs,
                         up_statement else_body);
    up_expression condition;
    up_statement body;
    up_else_if_vec else_ifs;
    up_statement else_body;
    void accept(Visitor& visitor) const override;
};

/**
 * @ingroup parser
 * @brief Else if statement representation.
 */
struct ElseIf : public Statement {
    explicit ElseIf(const Position& position, up_expression condition, up_statement body);
    up_expression condition;
    up_statement body;
    void accept(Visitor& visitor) const override;
};

/**
 * @ingroup parser
 * @brief Assignment statement representation.
 */
struct AssignStatement : public Statement {
    explicit AssignStatement(const Position& position, std::string identifier, up_expression expr);
    std::string identifier;
    up_expression expr;
    void accept(Visitor& visitor) const override;
};

/**
 * @ingroup parser
 * @brief Expression statement representation.
 */
struct ExpressionStatement : public Statement {
    explicit ExpressionStatement(up_expression expr);
    up_expression expr;
    void accept(Visitor& visitor) const override;
};

/**
 * @ingroup parser
 * @brief Function signature representation.
 */
struct FunctionSignature : public Node {
    explicit FunctionSignature(const Position& position, std::string identifier, up_typed_ident_vec params,
                               std::optional<Type> return_type);
    std::string identifier;
    Type type;
    up_typed_ident_vec params;
    void accept(Visitor& visitor) const override;

   private:
    void _deduce_function_type(std::optional<Type> return_type);
};

/**
 * @ingroup parser
 * @brief Function definition representation.
 */
struct FunctionDefinition : public Statement {
    explicit FunctionDefinition(up_func_sig signature, up_statement body);
    up_func_sig signature;
    up_statement body;
    void accept(Visitor& visitor) const override;
};

/**
 * @ingroup parser
 * @brief For loop representation.
 */
struct ForLoop : public Statement {
    explicit ForLoop(const Position& position, up_statement var_declaration, up_expression condition,
                     up_statement loop_update, up_statement body);
    up_statement var_declaration;
    up_expression condition;
    up_statement loop_update;
    up_statement body;

    void accept(Visitor& visitor) const override;
};

#endif  // STATEMENT_HPP