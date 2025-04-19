#include "position.hpp"

Position::Position(int line, int column) : _line{line}, _column{column} {}

Position Position::next_column() const {
    return Position{_line, _column + 1};
}

Position Position::next_line() const {
    return Position{_line + 1, 1};
}

int Position::get_line() const noexcept {
    return _line;
}

int Position::get_column() const noexcept {
    return _column;
}

std::string Position::get_position_str() const {
    return "[" + std::to_string(_line) + ":" + std::to_string(_column) + "]";
}

std::string Position::repr() const {
    return "Position(" + std::to_string(_line) + "," + std::to_string(_column) + ")";
}

std::ostream& operator<<(std::ostream& os, const Position& pos) {
    os << pos.repr();
    return os;
}