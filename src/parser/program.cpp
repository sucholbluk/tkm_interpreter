#include "program.hpp"

Program::Program(const Position& position, up_statement_vec statements)
    : Node{position}, statements{std::move(statements)} {};

void Program::accept(Visitor& visitor) const {
    visitor.visit(*this);
}