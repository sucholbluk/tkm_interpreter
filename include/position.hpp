#ifndef POSITION_HPP
#define POSITION_HPP
#include <ostream>
#include <string>

/*
 * @brief Position representation. Allowes to advance itself to the next column/line
 */
class Position {
   public:
    explicit Position(int line = 1, int column = 1);

    int get_line() const noexcept;
    int get_column() const noexcept;
    void next_column();
    void next_line();

    std::string get_position_str() const;
    std::string print() const;

    bool operator==(const Position& other) const noexcept;
    bool operator!=(const Position& other) const noexcept;

   private:
    int _line;
    int _column;
};

std::ostream& operator<<(std::ostream& os, const Position& pos);

#endif  // POSITION_HPP