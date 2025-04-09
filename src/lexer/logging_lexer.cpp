#include "logging_lexer.hpp"

#include <spdlog/spdlog.h>

const std::string LoggingLexer::_log_prefix{"[LEXER]"};

LoggingLexer::LoggingLexer(std::unique_ptr<ILexer> inner) : _inner{std::move(inner)} {}

Token LoggingLexer::get_next_token() {
    try {
        Token token{_inner->get_next_token()};
        _log_token_creation(token);
        return token;
    } catch (const std::exception& e) {
        _log_encountered_error(e);
        throw;  // just log and propagate to error handling object
    }
}

void LoggingLexer::_log_token_creation(const Token& token) {
    spdlog::info("{} Created Token: {}", _log_prefix, token.print());
}

void LoggingLexer::_log_encountered_error(const std::exception& e) {
    spdlog::error("{} Produced Error: {}", _log_prefix, e.what());
}