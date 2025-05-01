#ifndef NODE_HPP
#define NODE_HPP
#include "position.hpp"
#include "visitor.hpp"

struct Node {
    explicit Node(const Position& position);
    Position position;
    virtual ~Node() = default;

    virtual void accept(Visitor& visitor) const {};
};
#endif  // NODE_HPP