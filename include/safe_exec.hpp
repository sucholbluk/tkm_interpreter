#ifndef SAFE_EXEC_HPP
#define SAFE_EXEC_HPP
#include <functional>
/**
 * @ingroup app_core
 * @brief Module responsible for safe execution of the program - catching exceptions and providing feedback.
 * @brief Takes function to execute and handles errors that  can occure.
 */
namespace safe_exec {
/**
 * @ingroup app_core
 * @brief Wraps the program execution, handles exceptions.
 */
void run_safe(const std::function<void()>& task) noexcept;
}  // namespace safe_exec

#endif  // SAFE_EXEC_HPP