#ifndef IPARSER_HPP
#define IPARSER_HPP

#include "program.hpp"
/**
 * @ingroup parser
 * @brief Interface for parser component.
 */
class IParser {
   public:
    virtual std::unique_ptr<Program> parse_program() = 0;
    virtual ~IParser() = default;
};

#endif  // IPARSER_HPP