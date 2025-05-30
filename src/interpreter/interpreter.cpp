#include "interpreter.hpp"

#include "builtint_functions.hpp"
#include "global_function.hpp"
#include "program.hpp"
#include "statement.hpp"
#include "type_handler.hpp"

void Interpreter::visit(const Program& program) {
    std::ranges::for_each(program.function_definitions, [this](const auto& func_def) { func_def->accept(*this); });
    _execute_main();
}

void Interpreter::visit(const FunctionDefinition& func_def) {
    _env.register_function(func_def);
}

void Interpreter::visit(const FunctionCall& func_call) {
    func_call.callee->accept(*this);

    if (not TypeHandler::value_type_is<std::shared_ptr<Callable>>(_tmp_result)) {
        throw std::runtime_error("not callable");
    }
    auto func{TypeHandler::get_value_as<std::shared_ptr<Callable>>(_tmp_result)};
    auto func_type_info{func->get_type().function_type_info};
    arg_list arguments{_get_arg_list(func_call.argument_list)};
    _clear_tmp_result();

    if (not TypeHandler::args_match_params(arguments, func_type_info->param_types)) {
        throw std::runtime_error("arguments dont match required param types");
    }
    _env.calling_function();
    func->call(*this, arguments);

    _handle_function_call_end(func_type_info->return_type);
}

void Interpreter::visit(const Identifier& var_reference) {
    // sprawdzamy czy jest funkcja globalna, lub czy mamy taka zmienna
    if (auto global_func{_env.get_global_function(var_reference.name)}) {
        _tmp_result = global_func;
    } else if (auto opt_var_holder = _env.get_by_identifier(var_reference.name)) {
        _tmp_result = opt_var_holder.value();
    } else {
        throw std::runtime_error("xxxx is undefined");  // TODO
    }
}

void Interpreter::visit(const CodeBlock& code_block) {
    _env.entering_block();
    for (auto& statment : code_block.statements) {
        statment->accept(*this);
        if (_should_exit_code_block()) {
            break;
        }
    }
    _env.exiting_block();
}

void Interpreter::visit(const TypeCastExpression& type_cast_expr) {
    type_cast_expr.expr->accept(*this);
    if (_tmp_result_is_empty()) {
        throw std::runtime_error("expr evaluates to nothing - cannot be converted");
    }

    value unwraped_value{TypeHandler::extract_value(_tmp_result)};
    Type target_type{type_cast_expr.target_type};

    if (auto opt_casted = TypeHandler::as_type(target_type, unwraped_value)) {
        _tmp_result = opt_casted.value();
        return;
    }

    throw std::runtime_error("cant convert from source type to target");
}

void Interpreter::visit(const VariableDeclaration& var_decl) {
    var_decl.assigned_expression->accept(*this);
    if (_tmp_result_is_empty()) throw std::runtime_error("nothing to assign");  // TODO

    auto value_to_assign{TypeHandler::extract_value(_tmp_result)};
    auto var_type{var_decl.typed_identifier->type};
    if (TypeHandler::deduce_type(value_to_assign) != var_type.type) {
        throw std::runtime_error("in var delcaration - type mismatch");  // TOOD
    }
    _env.declare_variable(var_decl.typed_identifier->name, var_type, value_to_assign);
    _clear_tmp_result();
}

void Interpreter::visit(const AssignStatement& asgn_stmnt) {
    auto opt_var_holder{_env.get_by_identifier(asgn_stmnt.identifier)};
    if (not opt_var_holder) {
        throw std::runtime_error("reference to non-existant var");  // TODO
    }

    auto var_holder{opt_var_holder.value()};
    if (not var_holder.can_change_var) {
        throw std::runtime_error("cannot assign to immutable variable");
    }

    asgn_stmnt.expr->accept(*this);
    if (_tmp_result_is_empty()) throw std::runtime_error("nothing to assign");  // TODO

    auto value_to_assign{TypeHandler::extract_value(_tmp_result)};
    if (TypeHandler::deduce_type(value_to_assign) != var_holder.get_type()) {
        throw std::runtime_error("in assigning - type mismatch");  // TOOD
    }
    var_holder.var->var_value = value_to_assign;
    _clear_tmp_result();
}

void Interpreter::visit(const ExpressionStatement& expr_stmnt) {
    expr_stmnt.expr->accept(*this);
    _clear_tmp_result();
}

void Interpreter::visit(const ReturnStatement& return_stmnt) {
    if (return_stmnt.expression) return_stmnt.expression->accept(*this);
    _is_returning = true;
}

void Interpreter::visit(const LiteralString& literal_string) {
    _tmp_result = literal_string.value;
}

void Interpreter::visit(const LiteralInt& literal_int) {
    _tmp_result = literal_int.value;
}
void Interpreter::visit(const LiteralFloat& literal_float) {
    _tmp_result = literal_float.value;
}

void Interpreter::visit(const LiteralBool& literal_bool) {
    _tmp_result = literal_bool.value;
}

void Interpreter::_execute_main() {
    auto main{_env.get_global_function("main")};
    if (not main) throw std::runtime_error("No main function to execute");

    if (not(main->get_type() == MainProperties::type)) {
        throw std::runtime_error("invalid main function definition");
    };

    _env.calling_function();
    main->call(*this, {});
    _handle_function_call_end(main->get_type().function_type_info->return_type);
}

arg_list Interpreter::_get_arg_list(const up_expression_vec& arguments) {
    arg_list args{};
    std::ranges::for_each(arguments, [&](const up_expression& expr) {
        expr->accept(*this);

        if (_tmp_result_is_empty()) {
            throw std::logic_error("temp result should not be empty after evaluating expression");  // TODO
        }
        args.push_back(TypeHandler::maybe_value_to_arg(_tmp_result));
    });

    return args;
}

void Interpreter::_handle_function_call_end(const std::optional<Type>& ret_type) {
    if (not TypeHandler::matches_return_type(_tmp_result, ret_type)) {
        throw std::runtime_error("return type mismatch");  // TODO
    }

    // if function was returning something make sure for it to be a value not var_holder
    if (not _tmp_result_is_empty()) {
        _tmp_result = TypeHandler::extract_value(_tmp_result);
    }
    _is_returning = false;
    _env.exiting_function();
}

bool Interpreter::_tmp_result_is_empty() const {
    return std::holds_alternative<std::monostate>(_tmp_result);
}

bool Interpreter::_should_exit_code_block() const {
    return _is_returning;
}

void Interpreter::_clear_tmp_result() {
    _tmp_result = std::monostate{};
}