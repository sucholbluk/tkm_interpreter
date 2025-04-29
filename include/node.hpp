#ifndef NODE_HPP
#define NODE_HPP
#include "position.hpp"

struct Node {
    explicit Node(const Position& position);
    Position position;
    virtual ~Node() = default;
};
#endif  // NODE_HPP