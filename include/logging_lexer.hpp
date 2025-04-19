#ifndef LOGGING_LEXER_HPP
#define LOGGING_LEXER_HPP
#include <memory>
#include <stdexcept>

#include "ilexer.hpp"

/*
 * @brief Decorator for a lexer that adds logging functionality.
 */
class LoggingLexer : public ILexer {
   public:
    explicit LoggingLexer(std::unique_ptr<ILexer> inner);
    Token get_next_token() override;

   private:
    static const std::string _log_prefix;

    std::unique_ptr<ILexer> _inner;
    void _log_token_creation(const Token&);
    void _log_encountered_error(const std::exception& e);
};

#endif  // LOGGING_LEXER_HPP