#include "printer.hpp"

#include <iostream>
#include <ranges>

#include "program.hpp"

void Printer::_print_indent() const {
    std::cout << std::string(indent_level * 2, ' ');
}

void Printer::visit(const Program& program) {
    std::cout << "Program <" << this << "> at: " << program.position.get_position_str() << std::endl;
    _IndentGuard guard(indent_level);
    std::ranges::for_each(program.statements, [this](const up_statement& statement) {
        statement->accept(*this);
    });
}

void Printer::visit(const ContinueStatement& continue_stmnt) {
    _print_indent();
    std::cout << "ContinueStatement <" << &continue_stmnt << "> at: " << continue_stmnt.position.get_position_str() << std::endl;
    _IndentGuard guard(indent_level);
}

void Printer::visit(const BreakStatement& break_stmnt) {
    _print_indent();
    std::cout << "BreakStatement <" << &break_stmnt << "> at: " << break_stmnt.position.get_position_str() << std::endl;
}