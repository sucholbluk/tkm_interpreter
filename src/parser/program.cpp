#include "program.hpp"

Program::Program(const Position& position, up_statement_vec statements)
    : Node{position}, statements{std::move(statements)} {};
