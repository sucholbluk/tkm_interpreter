#include "callable.hpp"

std::ostream& operator<<(std::ostream& os, const Callable& callable) {
    os << std::format("Callable - {}]", callable.get_type().to_str());
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Callable>& ptr) {
    if (ptr)
        os << std::format("[sp to Callable - {}]", ptr->get_type().to_str());
    else
        os << "[sp to Callable - nullptr]";
    return os;
}
