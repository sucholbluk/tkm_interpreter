#include "safe_exec.hpp"

#include <spdlog/spdlog.h>

#include "exceptions.hpp"

void safe_exec::run_safe(const std::function<void()>& task) noexcept {
    try {
        spdlog::set_pattern("[%^%l%$] %v");
        task();
    } catch (const ImplementationError& e) {
        spdlog::error("[IMPLEMENTATION ERROR!!!]: {} msg: {}", typeid(e).name(), e.what());
    } catch (const LexerException& e) {
        spdlog::error("{} message: {}", typeid(e).name(), e.what());
    } catch (const ParserException& e) {
        spdlog::error("SyntaxError: {}", e.what());
    } catch (const InterpreterException& e) {
        spdlog::error("RuntimeError: {}", e.what());
    } catch (const FileOpenException& e) {
        spdlog::error("FileOpenException: {}", e.what());
    } catch (const std::exception& e) {
        spdlog::error(e.what());
    } catch (...) {
        spdlog::error("Unknown exception");
    }
}