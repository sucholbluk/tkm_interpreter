#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP
#include <memory>

#include "node.hpp"

struct Expression : Node {
    using Node::Node;
};

using up_expression = std::unique_ptr<Expression>;

#endif  // EXPRESSION_HPP
