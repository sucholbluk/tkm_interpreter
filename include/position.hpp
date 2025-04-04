#ifndef POSITION_H
#define POSITION_H
#include <ostream>
#include <string>

class Position {
    int _line;
    int _column;

   public:
    Position(int line = 1, int column = 1);

    int get_line() const;
    int get_column() const;
    void next_column();
    void next_line();

    std::string print() const;
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
};

std::ostream& operator<<(std::ostream& os, const Position& pos);

#endif  // POSITION_H