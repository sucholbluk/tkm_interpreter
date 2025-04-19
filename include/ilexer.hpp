#ifndef ILEXER_HPP
#define ILEXER_HPP

#include "token.hpp"
/*
 * @brief Interface for lexer component.
 */
class ILexer {
   public:
    virtual Token get_next_token() = 0;
    virtual ~ILexer() = default;
};

#endif  // ILEXER_HPP