#include <iostream>

#include "execution_guard.hpp"
#include "program.hpp"

int main(int argc, char* const argv[]) {
    ExecutionGuard::run([argc, argv]() {
        Program program(argc, argv);
        program.run();
    });

    return 0;
}
