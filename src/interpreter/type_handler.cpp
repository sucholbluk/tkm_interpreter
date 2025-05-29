#include "type_handler.hpp"

#include <algorithm>
#include <ranges>

#include "callable.hpp"

namespace TypeHandler {
Type deduce_type(value val) {
    return std::visit(
        []<typename T>(const T& _val) -> Type {
            if constexpr (std::same_as<int, T>) {
                return Type{TypeKind::INT};
            } else if constexpr (std::same_as<double, T>) {
                return Type{TypeKind::FLOAT};
            } else if constexpr (std::same_as<bool, T>) {
                return Type{TypeKind::BOOL};
            } else if constexpr (std::same_as<std::string, T>) {
                return Type{TypeKind::STRING};
            } else if constexpr (std::same_as<std::shared_ptr<Callable>, T>) {
                return _val->get_type();
            }
        },
        val);
}

bool args_match_params(arg_list args, std::vector<VariableType> param_types) {
    if (args.size() != param_types.size()) return false;

    return std::ranges::all_of(std::views::iota(size_t{0}, args.size()),
                               [&](size_t i) { return arg_matches_param(args[i], param_types[i]); });
}

bool arg_matches_param(arg argument, VariableType param_type) {
    return std::visit(
        [param_type]<typename T>(const T& argument) -> bool {
            if constexpr (std::same_as<value, T>) {
                return deduce_type(argument) == param_type.type;
            } else if constexpr (std::same_as<VariableHolder, T>) {
                if (param_type.is_mutable and not argument.can_change_var) return false;

                // mutable variable can be passed as immutable variable - the other way no
                return argument.get_type() == param_type.type;
            }
        },
        argument);
}

arg maybe_value_to_arg(const std::variant<std::monostate, VariableHolder, value>& maybe_val_or_holder) {
    return std::visit(
        []<typename T>(const T& v_or_vh) -> arg {
            if constexpr (std::same_as<T, VariableHolder>) {
                return v_or_vh;
            } else if constexpr (std::same_as<T, value>) {
                return v_or_vh;
            } else if constexpr (std::same_as<T, std::monostate>) {
                throw std::logic_error("impl err - check if not monostate before calling");
            }
        },
        maybe_val_or_holder);
}

}  // namespace TypeHandler
