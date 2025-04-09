#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <memory>

#include "ilexer.hpp"

class Program {  // nazwa tymczasowa - potem moze do zmiany
   public:
    Program(int argc, char* const argv[]);
    void run();

   private:
    std::unique_ptr<ILexer> _lexer;
    // TODO: make struct for options
    bool _verbose;
    bool _use_stdin;
    std::string _input_filename;
    std::string _output_filename;

    void _parse_args(int argc, char* const argv[]);
    void _initialize_components();
};

#endif  // PROGRAM_HPP