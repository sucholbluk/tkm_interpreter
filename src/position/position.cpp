#include "position.hpp"

Position::Position(int line, int column) : _line{line}, _column{column} {}

int Position::get_line() const {
    return _line;
}

int Position::get_column() const {
    return _column;
}

void Position::next_column() {
    ++_column;
}

void Position::next_line() {
    _column = 1;
    ++_line;
}

std::string Position::get_position() const {
    return "[" + std::to_string(_line) + ":" + std::to_string(_column) + "]";
}

std::string Position::print() const {
    return "Position(" + std::to_string(_line) + "," + std::to_string(_column) + ")";
}

bool Position::operator==(const Position& other) const {
    return this->_line == other._line && this->_column == other._column;
}

bool Position::operator!=(const Position& other) const {
    return not(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Position& pos) {
    os << pos.print();
    return os;
}