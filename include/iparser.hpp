#ifndef IPARSER_HPP
#define IPARSER_HPP

#include "program.hpp"
/**
 * @ingroup parser
 * @brief Interface for parser component.
 *
 * Provides an interface for parsing source code into an program representation.
 * Implementations should provide logic for parsing and returning built Program.
 */
class IParser {
   public:
    /**
     * @brief Parses the source code and returns the Program.
     * @return Unique pointer to the parsed Program
     */
    virtual std::unique_ptr<Program> parse_program() = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~IParser() = default;
};

#endif  // IPARSER_HPP