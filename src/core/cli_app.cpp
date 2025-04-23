#include "cli_app.hpp"

#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>

#include "lexer.hpp"
#include "logging_lexer.hpp"

namespace p_opt = boost::program_options;

CLIApp::CLIApp(int argc, char* const argv[]) : _use_stdin{false}, _verbose{false} {
    _parse_args(argc, argv);
    _initialize_components();
}

void CLIApp::run() {
    Token current_token{_lexer->get_next_token()};
    do {
        current_token = _lexer->get_next_token();
    } while (current_token.get_type() != TokenType::T_EOF);
}

void CLIApp::_parse_args(int argc, char* const argv[]) {
    p_opt::options_description desc{"available options:"};
    p_opt::options_description hidden{"positional arguments:"};
    p_opt::positional_options_description p;

    desc.add_options()  // clang-format off
        ("help,h", "display help info")
        ("stdin,s", p_opt::bool_switch(&_use_stdin), "read data from standard input")
        ("verbose,v", p_opt::bool_switch(&_verbose), "enable verbosity")
        ("output,o", p_opt::value<std::string>(&_output_filename), "output filename") // jeszcze nie obslugiwana
        ("input", p_opt::value<std::string>(&_input_filename), "input filename");  // clang-format on

    p.add("input", 1);

    p_opt::variables_map vm;
    p_opt::store(p_opt::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    p_opt::notify(vm);

    if (vm.count("help") or (_input_filename.empty() and not _use_stdin)) {
        std::cout << "usage: ./TKM_Compiler [file] [options]\n"
                  << desc << "\n";
        exit(0);
    }
}

void CLIApp::_initialize_components() {
    std::unique_ptr<std::istream> input_stream;
    // tymczasowe rozwiązanie
    if (_use_stdin) {
        input_stream = std::make_unique<std::istream>(std::cin.rdbuf());
    } else {
        auto file_stream = std::make_unique<std::ifstream>(_input_filename);
        if (!file_stream->is_open())
            throw FileOpenException(_input_filename);

        input_stream = std::move(file_stream);
    }

    std::unique_ptr<SourceHandler> source_handler = std::make_unique<SourceHandler>(std::move(input_stream));
    _lexer = std::make_unique<Lexer>(std::move(source_handler));
    if (_verbose) {
        _lexer = std::make_unique<LoggingLexer>(std::move(_lexer));
    }
}