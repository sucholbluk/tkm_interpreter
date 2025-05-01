#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <memory>
#include <vector>

#include "node.hpp"
#include "statement.hpp"

using up_statement_vec = std::vector<std::unique_ptr<Statement>>;

struct Program : public Node {
    Program(const Position& position, up_statement_vec statements);
    void accept(Visitor& visitor) const override;
    up_statement_vec statements;
};

#endif  // PROGRAM_HPP