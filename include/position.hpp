#ifndef POSITION_H
#define POSITION_H
#include <string>

class Position {
    int _line;
    int _column;

   public:
    Position(int line = 1, int column = 1) : _line{line}, _column{column} {}

    int get_line() const { return _line; };
    int get_column() const { return _column; };

    void next_column() { ++_column; }
    void next_line() {
        ++_line;
        _column = 1;
    }

    std::string print() const {
        return "[" + std::to_string(_column) + ":" + std::to_string(_line) + "]";
    }
};

#endif  // POSITION_H