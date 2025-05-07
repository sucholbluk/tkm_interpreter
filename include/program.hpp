#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <memory>
#include <vector>

#include "node.hpp"
#include "statement.hpp"

struct Program : public Node {
    Program(const Position& position, up_fun_def_vec function_definitions);
    void accept(Visitor& visitor) const override;
    up_fun_def_vec function_definitions;
};

#endif  // PROGRAM_HPP