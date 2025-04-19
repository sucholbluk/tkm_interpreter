#include <iostream>

#include "program.hpp"
#include "safe_exec.hpp"

int main(int argc, char* const argv[]) {
    safe_exec::run_safe([argc, argv]() {
        Program program(argc, argv);
        program.run();
    });

    return 0;
}
