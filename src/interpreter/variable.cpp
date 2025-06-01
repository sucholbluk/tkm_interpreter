#include "variable.hpp"

VariableHolder::VariableHolder(sp_variable var) : var{var}, can_change_var{var->type.is_mutable} {}

VariableHolder::VariableHolder(sp_variable var, bool can_change_var) : var{var}, can_change_var{can_change_var} {}

Type VariableHolder::get_type() const {
    return var->type.type;
}

std::ostream& operator<<(std::ostream& os, const value& v) {
    std::visit([&os](auto&& arg) { os << arg; }, v);
    return os;
}

std::ostream& operator<<(std::ostream& os, const VariableHolder& var_hold) {
    os << "Variable holder of: " << var_hold.var;
    return os;
}

std::ostream& operator<<(std::ostream& os, const vhold_or_val& a) {
    std::visit([&os](auto&& v) { os << v; }, a);
    return os;
}

std::ostream& operator<<(std::ostream& os, const arg_list& args) {
    os << "[";
    bool first = true;
    for (const auto& a : args) {
        if (!first) os << ", ";
        os << a;
        first = false;
    }
    os << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<VariableType>& v) {
    os << "[";
    bool first = true;
    for (const auto& elem : v) {
        if (!first) os << ", ";
        os << elem;
        first = false;
    }
    os << "]";
    return os;
}