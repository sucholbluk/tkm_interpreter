#include "source_handler.hpp"

SourceHandler::SourceHandler(std::unique_ptr<std::istream> source)
    : _source{std::move(source)}, _position{} {}

std::pair<char, Position> SourceHandler::get_char_and_position() {
    char current_char = _get_char();
    Position current_position = _position;
    _adjust_position(current_char);

    return {current_char, current_position};
}

char SourceHandler::_get_char() {
    char c;

    if (not _source->get(c))
        return EOF_CHAR;

    if (c == CR_CHAR and _source->peek() == LF_CHAR)
        _source->get(c);

    return c;
}

void SourceHandler::_adjust_position(char c) {
    if (c == LF_CHAR) {
        _position.next_line();
    } else if (c != EOF_CHAR) {
        _position.next_column();
    }
}
