#ifndef EXECUTION_GUARD_HPP
#define EXECUTION_GUARD_HPP
#include <functional>
/*
 * @brief Takes function to execute and handles errors that  can occure.
 */
class ExecutionGuard {
   public:
    static void run_safe(const std::function<void()>& task) noexcept;
};

#endif  // EXECUTION_HUARD_HPP