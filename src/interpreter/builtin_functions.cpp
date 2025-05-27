#include "builtint_functions.hpp"

BuiltinFunction::BuiltinFunction(Type type, function_impl& impl) : _type{type}, _impl{impl} {}
