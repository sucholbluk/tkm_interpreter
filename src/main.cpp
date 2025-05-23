#include <iostream>

#include "cli_app.hpp"
#include "safe_exec.hpp"

int main(int argc, char* const argv[]) {
    safe_exec::run_safe([argc, argv]() {
        CLIApp app(argc, argv);
        app.run();
    });

    return 0;
}
