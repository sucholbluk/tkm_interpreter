#ifndef SOURCE_HANDLER_HPP
#define SOURCE_HANDLER_HPP
#include <istream>
#include <memory>
#include <utility>

#include "constants.hpp"
#include "position.hpp"

/*
 * @bref Class providing chars for @ref Lexer and controlling the positon in stream.
 */
class SourceHandler {
   public:
    explicit SourceHandler(std::unique_ptr<std::istream> source);
    std::pair<char, Position> get_char_and_position();

   private:
    Position _position;
    std::unique_ptr<std::istream> _source;

    void _adjust_position(char c);
    char _get_char();
};

#endif  // SOURCE_HANDLER_HPP