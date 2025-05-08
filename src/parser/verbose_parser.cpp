#include "verbose_parser.hpp"

#include "printer.hpp"

VerboseParser::VerboseParser(std::unique_ptr<IParser> real_parser) : _real_parser{std::move(real_parser)} {}

std::unique_ptr<Program> VerboseParser::parse_program() {
    auto program = _real_parser->parse_program();
    Printer printer{};
    program->accept(printer);
    return program;
}