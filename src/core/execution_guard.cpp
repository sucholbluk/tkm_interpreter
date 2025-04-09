#include "execution_guard.hpp"

#include <spdlog/spdlog.h>

void ExecutionGuard::run(const std::function<void()>& task) noexcept {
    try {
        spdlog::set_pattern("[%^%l%$] %v");
        task();
    } catch (const std::exception& e) {
        spdlog::error(e.what());
    } catch (...) {
        spdlog::error("Unknown exception");
    }
}