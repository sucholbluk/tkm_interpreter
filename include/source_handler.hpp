#ifndef SOURCE_HANDLER_HPP
#define SOURCE_HANDLER_HPP
#include <istream>
#include <utility>

#include "constants.hpp"
#include "position.hpp"

class SourceHandler {
   public:
    explicit SourceHandler(std::istream& source);
    std::pair<char, Position> get_char_and_position();

   private:
    Position _position;
    std::istream& _source;

    void _adjust_position(char c);
    char _get_char();
};

#endif  // SOURCE_HANDLER_HPP