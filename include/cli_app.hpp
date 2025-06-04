#ifndef CLI_APP_HPP
#define CLI_APP_HPP

#include <memory>

#include "ilexer.hpp"
#include "interpreter.hpp"
#include "iparser.hpp"

/**
 * @defgroup app_core Application Core
 * @brief Module responsible for the CLI and handling program execution with information.
 */

/**
 * @ingroup app_core
 * @brief to represent program
 *
 * Perhaps to break into smaller pieces in future
 */
class CLIApp {  // nazwa tymczasowa - potem moze do zmiany
   public:
    CLIApp(int argc, char* const argv[]);
    void run();

   private:
    std::unique_ptr<IParser> _parser;
    // TODO: make struct for options
    bool _use_stdin;
    bool _verbose;
    Interpreter _interpreter;
    std::string _input_filename;

    void _parse_args(int argc, char* const argv[]);
    void _initialize_components();
};

#endif  // CLI_APP