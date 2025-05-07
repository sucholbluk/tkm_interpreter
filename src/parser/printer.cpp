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

void Printer::_print_expression_header(const Expression& expr, std::string type_spec,
                                       std::string additional_info) const {
    _print_indent();
    std::cout << "[\033[1;34m" << expr_kind_to_str(expr.kind) << type_spec << "\033[0m]"
              << " <" << &expr << "> at:" << expr.position.get_position_str() << "" << additional_info << std::endl;
}

void Printer::_print_header(std::string type_str, const Node& node, std::string additional_info) const {
    std::cout << "[\033[1;32m" << type_str << "\033[0m"
              << "] <" << &node << "> at:" << node.position.get_position_str() << "" << additional_info << std::endl;
}
/* -----------------------------------------------------------------------------*
 *                               PRINTING STATEMENTS                            *
 *------------------------------------------------------------------------------*/

void Printer::visit(const Program& program) {
    _print_header("Program", program);
    _IndentGuard guard(_indent_level);
    std::ranges::for_each(program.statements, [this](const up_statement& statement) { statement->accept(*this); });
}

void Printer::visit(const ContinueStatement& continue_stmnt) {
    _print_indent();
    _print_header("ContinueStatement", continue_stmnt);
}

void Printer::visit(const BreakStatement& break_stmnt) {
    _print_indent();
    _print_header("BreakStatement", break_stmnt);
}

void Printer::visit(const ReturnStatement& return_stmnt) {
    _print_indent();
    _print_header("ReturnStatement", return_stmnt);
    _IndentGuard guard{_indent_level};
    if (return_stmnt.expression) {
        return_stmnt.expression->accept(*this);
    }
}

void Printer::visit(const VariableDeclaration& var_decl) {
    _print_indent();
    _print_header("VariableDeclaration", var_decl);
    _IndentGuard guard{_indent_level};

    var_decl.typed_identifier->accept(*this);
    var_decl.assigned_expression->accept(*this);
}

void Printer::visit(const CodeBlock& code_block) {
    _print_indent();
    _print_header("CodeBlock", code_block);
    {
        _IndentGuard guard{_indent_level};
        std::ranges::for_each(code_block.statements,
                              [this](const up_statement& statement) { statement->accept(*this); });
    }
}

void Printer::visit(const IfStatement& if_stmnt) {
    _print_indent();
    _print_header("IfStatement", if_stmnt);
    {
        _IndentGuard guard{_indent_level};
        _print_indent();
        std::cout << "Contition:" << std::endl;
        {
            _IndentGuard condition_guard{_indent_level};
            if_stmnt.condition->accept(*this);
        }
        _print_indent();
        std::cout << "Body:" << std::endl;
        {
            _IndentGuard body_guard{_indent_level};
            if_stmnt.body->accept(*this);
        }

        if (not if_stmnt.else_ifs.empty()) {
            _print_indent();
            std::cout << "Else Ifs:" << std::endl;
            _IndentGuard else_ifs_guard{_indent_level};
            std::ranges::for_each(if_stmnt.else_ifs, [this](const up_else_if& else_if) { else_if->accept(*this); });
        }

        if (if_stmnt.else_body) {
            _print_indent();
            std::cout << "Else:" << std::endl;
            _IndentGuard else_guard{_indent_level};
            if_stmnt.else_body->accept(*this);
        }
    }
}

void Printer::visit(const ElseIf& else_if) {
    _print_indent();
    _print_header("ElseIf", else_if);
    _IndentGuard guard{_indent_level};
    _print_indent();
    std::cout << "Condition:" << std::endl;
    {
        _IndentGuard condition_guard{_indent_level};
        else_if.condition->accept(*this);
    }
    _print_indent();
    std::cout << "Body:" << std::endl;
    {
        _IndentGuard body_guard{_indent_level};
        else_if.body->accept(*this);
    }
}

void Printer::visit(const AssignStatement& asgn_stmnt) {
    _print_indent();
    _print_header("AssignStatement", asgn_stmnt, std::format(" to identifier: {}", asgn_stmnt.identifier));
    _IndentGuard guard{_indent_level};
    asgn_stmnt.expr->accept(*this);
}

void Printer::visit(const ExpressionStatement& expr_stmnt) {
    _print_indent();
    _print_header("ExpressionStatement", expr_stmnt);
    _IndentGuard guard{_indent_level};
    expr_stmnt.expr->accept(*this);
}

void Printer::visit(const FunctionDefinition& func_def) {
    _print_indent();
    _print_header("FunctionDefinition", func_def);
    _IndentGuard guard{_indent_level};
    func_def.signature->accept(*this);
    func_def.body->accept(*this);
}

void Printer::visit(const FunctionSignature& func_sig) {
    _print_indent();
    _print_header("FunctionSignature", func_sig,
                  std::format(" identifier: {}, type: {}", func_sig.identifier, func_sig.type.to_str()));
    _IndentGuard guard{_indent_level};

    std::ranges::for_each(func_sig.params, [this](const up_typed_identifier& param) { param->accept(*this); });
}

void Printer::visit(const ForLoop& for_loop) {
    _print_indent();
    _print_header("ForLoop", for_loop);
    _IndentGuard loop_guard{_indent_level};
    _print_indent();
    std::cout << "LoopVarDeclaration:" << std::endl;
    {
        _IndentGuard loop_var_guard{_indent_level};
        for_loop.var_declaration->accept(*this);
    }
    _print_indent();
    std::cout << "Condition:" << std::endl;
    {
        _IndentGuard condition_guard{_indent_level};
        for_loop.condition->accept(*this);
    }
    _print_indent();
    std::cout << "LoopUpdate:" << std::endl;
    {
        _IndentGuard loop_update_guard{_indent_level};
        for_loop.condition->accept(*this);
    }
    _print_indent();
    std::cout << "Body:" << std::endl;
    {
        _IndentGuard body_guard{_indent_level};
        for_loop.body->accept(*this);
    }
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
    type_cast_expr.expr->accept(*this);
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
    _print_header("TypedIdentifier", typed_identifier,
                  std::format(" identifier:{}, type:{}", typed_identifier.name, typed_identifier.type.to_str()));
}
