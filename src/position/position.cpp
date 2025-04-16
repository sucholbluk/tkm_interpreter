#include "position.hpp"

void Position::next_column() {
    ++column;
}

void Position::next_line() {
    column = 1;
    ++line;
}

std::string Position::get_position_str() const {
    return "[" + std::to_string(line) + ":" + std::to_string(column) + "]";
}

std::string Position::repr() const {
    return "Position(" + std::to_string(line) + "," + std::to_string(column) + ")";
}

std::ostream& operator<<(std::ostream& os, const Position& pos) {
    os << pos.repr();
    return os;
}