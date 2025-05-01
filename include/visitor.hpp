#ifndef VISITIOR_HPP
#define VISITIOR_HPP

struct Program;
struct ContinueStatement;
struct BreakStatement;

class Visitor {
   public:
    virtual ~Visitor() = default;

    virtual void visit(const Program& program) = 0;
    virtual void visit(const ContinueStatement& program) = 0;
    virtual void visit(const BreakStatement& program) = 0;
};

#endif  // VISITOR_HPP