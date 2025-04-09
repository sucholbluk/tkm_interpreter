#ifndef EXECUTION_GUARD_HPP
#define EXECUTION_GUARD_HPP
#include <functional>

class ExecutionGuard {
   public:
    static void run(const std::function<void()>& task) noexcept;
};

#endif  // EXECUTION_HUARD_HPP