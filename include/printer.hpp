#ifndef PRINTER_HPP
#define PRINTER_HPP

#include "visitor.hpp"

class Printer : public Visitor {
   public:
    void visit(const Program& program) override;
    void visit(const ContinueStatement& program) override;
    void visit(const BreakStatement& program) override;
    ~Printer() = default;

   private:
    int indent_level = 0;
    void _print_indent() const;

    struct _IndentGuard {
        int& level;
        explicit _IndentGuard(int& level) : level{level} { ++level; }
        ~_IndentGuard() { --level; }
    };
};

#endif  // PRINTER_HPP
