#ifndef SAFE_EXEC_HPP
#define SAFE_EXEC_HPP
#include <functional>
/*
 * @brief Takes function to execute and handles errors that  can occure.
 */
namespace safe_exec {
void run_safe(const std::function<void()>& task) noexcept;
}

#endif  // SAFE_EXEC_HPP