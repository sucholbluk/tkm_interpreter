#include "program.hpp"

Program::Program(const Position& position, up_fun_def_vec function_definitions)
    : Node{position}, function_definitions{std::move(function_definitions)} {};

void Program::accept(Visitor& visitor) const {
    visitor.visit(*this);
}