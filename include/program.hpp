#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <memory>
#include <vector>

#include "ast_node.hpp"
#include "statement.hpp"

using up_statement_vec = std::vector<std::unique_ptr<Statement>>;

class Program : public ASTNode {
   public:
    Program(up_statement_vec statements);
    const up_statement_vec& get_statements();

   private:
    up_statement_vec _statements;
};

#endif  // PROGRAM_HPP