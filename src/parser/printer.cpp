#include "printer.hpp"

#include <format>
#include <iostream>
#include <ranges>

#include "expression.hpp"
#include "program.hpp"
#include "typed_identifier.hpp"

void Printer::_print_indent() const {
    std::cout << std::string(_indent_level * 2, ' ') << "|_";
}

/* -----------------------------------------------------------------------------*
 *                               PRINTING STATEMENTS                            *
 *------------------------------------------------------------------------------*/

void Printer::visit(const Program& program) {
    std::cout << "Program <" << this << "> at: " << program.position.get_position_str() << std::endl;
    _IndentGuard guard(_indent_level);
    std::ranges::for_each(program.statements, [this](const up_statement& statement) { statement->accept(*this); });
}

void Printer::visit(const ContinueStatement& continue_stmnt) {
    _print_indent();
    std::cout << "ContinueStatement <" << &continue_stmnt << "> at: " << continue_stmnt.position.get_position_str()
              << std::endl;
}

void Printer::visit(const BreakStatement& break_stmnt) {
    _print_indent();
    std::cout << "BreakStatement <" << &break_stmnt << "> at: " << break_stmnt.position.get_position_str() << std::endl;
}

void Printer::visit(const ReturnStatement& return_stmnt) {
    _print_indent();
    std::cout << "ReturnStatement <" << &return_stmnt << "> at: " << return_stmnt.position.get_position_str()
              << std::endl;
    _IndentGuard guard{_indent_level};
    if (return_stmnt.expression) {
        return_stmnt.expression->accept(*this);
    }
}

void Printer::visit(const VariableDeclarationStatement& var_decl) {
    _print_indent();
    std::cout << "VariableDeclarationStatement <" << &var_decl << "> at: " << var_decl.position.get_position_str()
              << std::endl;
    _IndentGuard guard{_indent_level};

    var_decl.typed_identifier->accept(*this);
    var_decl.assigned_expression->accept(*this);
}

/* -----------------------------------------------------------------------------*
 *                               PRINTING EXPRESSIONS                           *
 *------------------------------------------------------------------------------*/

void Printer::visit(const BinaryExpression& binary_expr) {
    _print_expression_header(binary_expr);
    _IndentGuard guard{_indent_level};
    binary_expr.left->accept(*this);
    binary_expr.right->accept(*this);
}

void Printer::visit(const UnaryExpression& unary_expr) {
    _print_expression_header(unary_expr);
    _IndentGuard guard{_indent_level};
    unary_expr.expr->accept(*this);
}

void Printer::visit(const TypeCastExpression& type_cast_expr) {
    _print_expression_header(type_cast_expr, "", std::format("target_type:{}", type_cast_expr.target_type.to_str()));
    _IndentGuard guard{_indent_level};
}

void Printer::visit(const FunctionCall& func_call_expr) {
    _print_expression_header(func_call_expr);
    _IndentGuard guard{_indent_level};

    _print_indent();
    std::cout << "Calle:" << std::endl;
    {
        _IndentGuard calle_guard{_indent_level};
        func_call_expr.callee->accept(*this);
    }

    _print_indent();
    std::cout << "Arguments:";
    if (func_call_expr.argument_list.empty()) {
        std::cout << "no arguments called" << std::endl;
    } else {
        std::cout << std::endl;
        _IndentGuard arguments_guard{_indent_level};
        std::ranges::for_each(func_call_expr.argument_list,
                              [this](const up_expression& argument) { argument->accept(*this); });
    }
}

void Printer::visit(const BindFront& bind_front_expr) {
    _print_expression_header(bind_front_expr);
    _IndentGuard guard{_indent_level};

    _print_indent();
    std::cout << "Arguments:" << std::endl;
    {
        _IndentGuard arguments_guard{_indent_level};
        std::ranges::for_each(bind_front_expr.argument_list,
                              [this](const up_expression& argument) { argument->accept(*this); });
    }

    _print_indent();
    std::cout << "Bind Target:" << std::endl;
    {
        _IndentGuard target_guard{_indent_level};
        bind_front_expr.target->accept(*this);
    }
}

void Printer::visit(const ParenExpression& paren_expr) {
    _print_expression_header(paren_expr);
    _IndentGuard guard{_indent_level};
    paren_expr.expr->accept(*this);
}

void Printer::visit(const Identifier& identifier) {
    _print_expression_header(identifier, "", std::format("name:{}", identifier.name));
}
void Printer::visit(const LiteralInt& literal_int) {
    _print_expression_header(
        literal_int, "Int",
        std::format("type:{}, value:{}", literal_int.type.to_str(), std::to_string(literal_int.value)));
}

void Printer::visit(const LiteralFloat& literal_float) {
    _print_expression_header(
        literal_float, "Float",
        std::format("type:{}, value:{}", literal_float.type.to_str(), std::to_string(literal_float.value)));
}
void Printer::visit(const LiteralString& literal_string) {
    _print_expression_header(literal_string, "String",
                             std::format(R"(type:{}, value:"{}")", literal_string.type.to_str(), literal_string.value));
}
void Printer::visit(const LiteralBool& literal_bool) {
    _print_expression_header(
        literal_bool, "Bool",
        std::format("type:{}, value:{}", literal_bool.type.to_str(), literal_bool.value ? "true" : "false"));
}

void Printer::visit(const TypedIdentifier& typed_identifier) {
    _print_indent();
    std::cout << "TypedIdentifier <" << &typed_identifier << "> at:" << typed_identifier.position.get_position_str()
              << std::format(" identifier:{}, type:{}", typed_identifier.name, typed_identifier.type.to_str())
              << std::endl;
}

void Printer::_print_expression_header(const Expression& expr, std::string type_spec,
                                       std::string additional_info) const {
    _print_indent();
    std::cout << expression_kind_to_str(expr.kind) << type_spec << " <" << &expr
              << "> at: " << expr.position.get_position_str() << " " << additional_info << std::endl;
}
