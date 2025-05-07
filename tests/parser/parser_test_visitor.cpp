#include "parser_test_visitor.hpp"

#include "program.hpp"
#include "statement.hpp"
#include "typed_identifier.hpp"

void ParserTestVisitor::visit(const Program& program) {
    elements.emplace_back(_get_name_position_str(program, "Program"), _nest_level);
    _NestGuard guard{_nest_level};
    std::ranges::for_each(program.statements, [this](const auto& stmnt) { stmnt->accept(*this); });
}

void ParserTestVisitor::visit(const ContinueStatement& continue_stmnt) {
    elements.emplace_back(_get_name_position_str(continue_stmnt, "ContinueStatement"), _nest_level);
}

void ParserTestVisitor::visit(const BreakStatement& break_stmnt) {
    elements.emplace_back(_get_name_position_str(break_stmnt, "BreakStatement"), _nest_level);
}

void ParserTestVisitor::visit(const ReturnStatement& return_stmnt) {
    elements.emplace_back(_get_name_position_str(return_stmnt, "ReturnStatement"), _nest_level);
    _NestGuard guard{_nest_level};
    return_stmnt.expression->accept(*this);
}

void ParserTestVisitor::visit(const VariableDeclaration& var_decl) {
    elements.emplace_back(_get_name_position_str(var_decl, "VariableDeclaration"), _nest_level);
    _NestGuard guard{_nest_level};
    var_decl.typed_identifier->accept(*this);
    var_decl.assigned_expression->accept(*this);
}

void ParserTestVisitor::visit(const CodeBlock& code_block) {
    elements.emplace_back(_get_name_position_str(code_block, "CodeBlock"), _nest_level);
    _NestGuard guard{_nest_level};
    std::ranges::for_each(code_block.statements, [this](const auto& stmnt) { stmnt->accept(*this); });
}

void ParserTestVisitor::visit(const IfStatement& if_stmnt) {
    elements.emplace_back(_get_name_position_str(if_stmnt, "IfStatement"), _nest_level);
    _NestGuard guard{_nest_level};
    if_stmnt.condition->accept(*this);
    if_stmnt.body->accept(*this);
    std::ranges::for_each(if_stmnt.else_ifs, [this](const auto& stmnt) { stmnt->accept(*this); });
    if (if_stmnt.else_body) {
        if_stmnt.else_body->accept(*this);
    }
}

void ParserTestVisitor::visit(const ElseIf& else_if) {
    elements.emplace_back(_get_name_position_str(else_if, "ElseIf"), _nest_level);
    _NestGuard guard{_nest_level};
    else_if.condition->accept(*this);
    else_if.body->accept(*this);
}

void ParserTestVisitor::visit(const AssignStatement& asgn_stmnt) {
    std::string assign_str{_get_name_position_str(asgn_stmnt, "AssignStatement") +
                           std::format(";to={}", asgn_stmnt.identifier)};
    elements.emplace_back(assign_str, _nest_level);
    _NestGuard guard{_nest_level};
    asgn_stmnt.expr->accept(*this);
}

void ParserTestVisitor::visit(const ExpressionStatement& expr_stmnt) {
    elements.emplace_back(_get_name_position_str(expr_stmnt, "ExpressionStatement"), _nest_level);
    _NestGuard guard{_nest_level};
    expr_stmnt.expr->accept(*this);
}

void ParserTestVisitor::visit(const FunctionDefinition& func_def) {
    elements.emplace_back(_get_name_position_str(func_def, "FunctionDefinition"), _nest_level);
    _NestGuard guard{_nest_level};
    func_def.signature->accept(*this);
    func_def.body->accept(*this);
}

void ParserTestVisitor::visit(const FunctionSignature& func_sig) {
    std::string func_sig_str{_get_name_position_str(func_sig, "FunctionSignature") +
                             std::format(";type={},identifier={}", func_sig.type.to_str(), func_sig.identifier)};
    elements.emplace_back(func_sig_str, _nest_level);
    _NestGuard guard{_nest_level};
    std::ranges::for_each(func_sig.params, [this](const auto& typed_ident) { typed_ident->accept(*this); });
}

void ParserTestVisitor::visit(const ForLoop& for_loop) {
    elements.emplace_back(_get_name_position_str(for_loop, "ForLoop"), _nest_level);
    _NestGuard guard{_nest_level};
    for_loop.var_declaration->accept(*this);
    for_loop.condition->accept(*this);
    for_loop.loop_update->accept(*this);
    for_loop.body->accept(*this);
}

void ParserTestVisitor::visit(const BinaryExpression& binary_expr) {
    elements.emplace_back(_get_name_position_str(binary_expr, expr_kind_to_str(binary_expr.kind)), _nest_level);
    _NestGuard guard{_nest_level};
    binary_expr.left->accept(*this);
    binary_expr.right->accept(*this);
}

void ParserTestVisitor::visit(const UnaryExpression& unary_expr) {
    elements.emplace_back(_get_name_position_str(unary_expr, expr_kind_to_str(unary_expr.kind)), _nest_level);
    _NestGuard guard{_nest_level};
    unary_expr.expr->accept(*this);
}

void ParserTestVisitor::visit(const FunctionCall& func_call_expr) {
    elements.emplace_back(_get_name_position_str(func_call_expr, expr_kind_to_str(func_call_expr.kind)), _nest_level);
    _NestGuard guard{_nest_level};
    func_call_expr.callee->accept(*this);
    std::ranges::for_each(func_call_expr.argument_list, [this](const auto& arg) { arg->accept(*this); });
}

void ParserTestVisitor::visit(const BindFront& bind_front_expr) {
    elements.emplace_back(_get_name_position_str(bind_front_expr, expr_kind_to_str(bind_front_expr.kind)), _nest_level);
    _NestGuard guard{_nest_level};
    std::ranges::for_each(bind_front_expr.argument_list, [this](const auto& arg) { arg->accept(*this); });
    bind_front_expr.target->accept(*this);
}

void ParserTestVisitor::visit(const TypeCastExpression& type_cast_expr) {
    std::string type_cast_str{_get_name_position_str(type_cast_expr, expr_kind_to_str(type_cast_expr.kind)) +
                              std::format(";to_type={}", type_cast_expr.target_type.to_str())};
    elements.emplace_back(type_cast_str, _nest_level);
    _NestGuard guard{_nest_level};
    type_cast_expr.expr->accept(*this);
}

void ParserTestVisitor::visit(const Identifier& identifier) {
    std::string ident_str{_get_name_position_str(identifier, expr_kind_to_str(identifier.kind)) +
                          std::format(";name={}", identifier.name)};
    elements.emplace_back(ident_str, _nest_level);
}
void ParserTestVisitor::visit(const LiteralInt& literal_int) {
    std::string lit_int_str{_get_name_position_str(literal_int, expr_kind_to_str(literal_int.kind) + "Int") +
                            std::format(";value={}", literal_int.value)};
    elements.emplace_back(lit_int_str, _nest_level);
}
void ParserTestVisitor::visit(const LiteralFloat& literal_float) {
    std::string lit_flt_str{_get_name_position_str(literal_float, expr_kind_to_str(literal_float.kind) + "Float") +
                            std::format(";value={}", literal_float.value)};

    elements.emplace_back(lit_flt_str, _nest_level);
}
void ParserTestVisitor::visit(const LiteralString& literal_string) {
    std::string lit_string_str{
        _get_name_position_str(literal_string, expr_kind_to_str(literal_string.kind) + "String") +
        std::format(R"(;value="{}")", literal_string.value)};

    elements.emplace_back(lit_string_str, _nest_level);
}
void ParserTestVisitor::visit(const LiteralBool& literal_bool) {
    std::string lit_bool_str{_get_name_position_str(literal_bool, expr_kind_to_str(literal_bool.kind) + "Bool") +
                             std::format(";value={}", literal_bool.value ? "true" : "false")};
    elements.emplace_back(lit_bool_str, _nest_level);
}

void ParserTestVisitor::visit(const TypedIdentifier& typed_ident) {
    std::string typed_ident_str{_get_name_position_str(typed_ident, "TypedIdentifier") +
                                std::format(";type={},name={}", typed_ident.type.to_str(), typed_ident.name)};
    elements.emplace_back(typed_ident_str, _nest_level);
}

std::string ParserTestVisitor::_get_name_position_str(const Node& node, std::string element_name) const {
    return std::format("{};{}", element_name, node.position.get_position_str());
}
