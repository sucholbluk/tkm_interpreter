#ifndef POSITION_HPP
#define POSITION_HPP
#include <compare>
#include <ostream>
#include <string>

/**
 * @defgroup position Position
 * @brief Module responsible for position representation.
 */

/**
 * @ingroup position
 * @brief Position representation. Allowes to advance itself to the next column/line
 */
class Position {
   public:
    Position(int line = 1, int column = 1);
    Position next_column() const;
    Position next_line() const;
    int get_line() const noexcept;
    int get_column() const noexcept;
    std::string repr() const;
    std::string get_position_str() const;

    auto operator<=>(const Position&) const = default;

   private:
    int _line;
    int _column;
};

std::ostream& operator<<(std::ostream& os, const Position& pos);

#endif  // POSITION_HPP