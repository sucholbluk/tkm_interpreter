#ifndef VERBOSE_PARSER_HPP
#define VERBOSE_PARSER_HPP

#include "iparser.hpp"
class VerboseParser : public IParser {
   public:
    explicit VerboseParser(std::unique_ptr<IParser> real_parser);
    std::unique_ptr<Program> parse_program() override;

   private:
    std::unique_ptr<IParser> _real_parser;
};

#endif  // VERBOSE_PARSER_HPP