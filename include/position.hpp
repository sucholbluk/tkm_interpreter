#ifndef POSITION_HPP
#define POSITION_HPP
#include <compare>
#include <ostream>
#include <string>

/*
 * @brief Position representation. Allowes to advance itself to the next column/line
 */
struct Position {
    int line = 1;
    int column = 1;

    void next_column();
    void next_line();
    std::string repr() const;
    std::string get_position_str() const;

    auto operator<=>(const Position&) const = default;
};

std::ostream& operator<<(std::ostream& os, const Position& pos);

#endif  // POSITION_HPP