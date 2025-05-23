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

AssignStatement::AssignStatement(const Position& position, std::string identifier, up_expression expr)
    : Statement{position}, identifier{identifier}, expr{std::move(expr)} {}

void AssignStatement::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

ExpressionStatement::ExpressionStatement(up_expression expr) : Statement{expr->position}, expr{std::move(expr)} {}

void ExpressionStatement::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

FunctionDefinition::FunctionDefinition(up_func_sig signature, up_statement body)
    : Statement{signature->position}, signature{std::move(signature)}, body{std::move(body)} {}

void FunctionDefinition::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

FunctionSignature::FunctionSignature(const Position& position, std::string identifier, up_typed_ident_vec params,
                                     std::optional<Type> return_type)
    : Node{position}, identifier{identifier}, params{std::move(params)} {
    _deduce_function_type(return_type);
}

void FunctionSignature::_deduce_function_type(std::optional<Type> return_type) {
    std::vector<VariableType> param_types{};

    for (const auto& up_param : params) {
        param_types.push_back(VariableType{up_param->type});
    }
    type = Type{FunctionTypeInfo{param_types, return_type}};
}

void FunctionSignature::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

ForLoop::ForLoop(const Position& position, up_statement var_declaration, up_expression condition,
                 up_statement loop_update, up_statement body)
    : Statement{position},
      var_declaration{std::move(var_declaration)},
      condition{std::move(condition)},
      loop_update{std::move(loop_update)},
      body{std::move(body)} {}

void ForLoop::accept(Visitor& visitor) const {
    visitor.visit(*this);
}