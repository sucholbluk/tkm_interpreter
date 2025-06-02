#include "interpreter.hpp"

#include "builtint_functions.hpp"
#include "exceptions.hpp"
#include "global_function.hpp"
#include "oper_handler.hpp"
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

    if (not TypeHandler::value_type_is<sp_callable>(_tmp_result)) {
        throw std::runtime_error("not callable");
    }
    auto func{TypeHandler::get_value_as<sp_callable>(_tmp_result)};
    auto func_type_info{func->get_type().function_type_info};
    arg_list arguments{_get_arg_list(func_call.argument_list)};
    _clear_tmp_result();

    if (not TypeHandler::args_match_params(arguments, func_type_info->param_types)) {
        throw std::runtime_error("arguments dont match required param types");
    }
    _env.calling_function(func_type_info->return_type);
    func->call(*this, arguments);
    _handle_function_call_end();
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
    _env.add_scope();
    for (auto& statment : code_block.statements) {
        statment->accept(*this);
        if (_should_exit_code_block()) {
            break;
        }
    }
    _env.pop_scope();
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

    if (not TypeHandler::matches_return_type(_tmp_result, _env.get_cur_func_ret_type())) {
        throw std::runtime_error("return type mismatch");  // TODO
    }

    // if function returns something make sure for it to be a value not var holder
    if (not _tmp_result_is_empty()) {
        _tmp_result = TypeHandler::extract_value(_tmp_result);
    }

    _is_returning = true;
}

void Interpreter::visit(const ContinueStatement& continue_stmnt) {
    if (not _inside_loop) {
        throw std::runtime_error("continue statement outside loop exept");  // TODO
    }
    _on_continue = true;
}

void Interpreter::visit(const BreakStatement& break_stmnt) {
    if (not _inside_loop) {
        throw std::runtime_error("break statement outside loop exept");  // TODO
    }
    _on_break = true;
}

void Interpreter::visit(const ForLoop& for_loop) {
    _enter_loop();
    for_loop.var_declaration->accept(*this);

    for_loop.condition->accept(*this);
    _evaluate_condition(for_loop.condition->position);

    while (_condition_met) {
        _on_continue = false;
        for_loop.body->accept(*this);
        if (_on_break or _is_returning) break;
        for_loop.loop_update->accept(*this);

        for_loop.condition->accept(*this);
        _evaluate_condition(for_loop.condition->position);
    }
    _exit_loop();
}

void Interpreter::visit(const BinaryExpression& binary_expr) {
    binary_expr.left->accept(*this);
    if (_tmp_result_is_empty()) {
        throw std::runtime_error("expected evaluable expression in binary expression, got none");  // with left position
    }
    auto left{TypeHandler::extract_value(_tmp_result)};
    _clear_tmp_result();

    binary_expr.right->accept(*this);
    if (_tmp_result_is_empty()) {
        throw std::runtime_error("expected evaluable expression in binary expression, got none");  // with right pos
    }
    auto right{TypeHandler::extract_value(_tmp_result)};
    _clear_tmp_result();
    try {
        _evaluate_binary_expr(binary_expr.kind, left, right);
    } catch (const std::runtime_error& e) {
        rethrow_with_position(e, binary_expr.position);
    }
}

void Interpreter::visit(const UnaryExpression& unary_expr) {
    unary_expr.expr->accept(*this);
    if (_tmp_result_is_empty()) {
        throw std::runtime_error("expected evaluable expression in unary expression, got none");  // with right pos
    }
    _evaluate_unary_expr(unary_expr.kind, TypeHandler::extract_value(_tmp_result));
}

void Interpreter::visit(const BindFront& bind_front_expr) {
    auto args{_get_arg_list(bind_front_expr.argument_list)};
    bind_front_expr.target->accept(*this);

    if (_tmp_result_is_empty()) {
        throw std::runtime_error("expected evaluable expression in unary expression, got none");  // with right pos
    }
    try {
        _tmp_result = OperHandler::bind_front_function(TypeHandler::extract_value(_tmp_result), args);
    } catch (const std::runtime_error& e) {
        rethrow_with_position(e, bind_front_expr.position);
    }
}

void Interpreter::visit(const IfStatement& if_stmnt) {
    if_stmnt.condition->accept(*this);
    _evaluate_condition(if_stmnt.condition->position);

    if (_condition_met) {
        if_stmnt.body->accept(*this);
    } else {
        for (size_t i = 0; i < if_stmnt.else_ifs.size(); ++i) {
            if_stmnt.else_ifs[i]->accept(*this);
            if (_condition_met) break;
        }

        if (not _condition_met and if_stmnt.else_body) {
            if_stmnt.else_body->accept(*this);
        }
    }
    _condition_met = false;
}

void Interpreter::visit(const ElseIf& else_if) {
    else_if.condition->accept(*this);
    _evaluate_condition(else_if.condition->position);

    if (_condition_met) else_if.body->accept(*this);
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

    _env.calling_function(main->get_type().function_type_info->return_type);
    main->call(*this, {});
    _handle_function_call_end();
}

arg_list Interpreter::_get_arg_list(const up_expression_vec& arguments) {
    arg_list args{};
    std::ranges::for_each(arguments, [&](const up_expression& expr) {
        expr->accept(*this);

        if (_tmp_result_is_empty()) {
            // expr->position();
            throw std::runtime_error("none not accepted as argument");
        }
        args.push_back(TypeHandler::opt_value_to_arg(_tmp_result));
    });

    return args;
}

void Interpreter::_handle_function_call_end() {
    _is_returning = false;
    _env.exiting_function();
}

void Interpreter::_evaluate_binary_expr(const ExprKind& expr_kind, value left, value right) {
    if (not TypeHandler::are_the_same_type(left, right)) {  // values have to be the same type
        throw std::runtime_error("type mismatch - // with type strings");
    }

    switch (expr_kind) {
        case ExprKind::ADDITION:
            _tmp_result = OperHandler::add(left, right);
            break;
        case ExprKind::SUBTRACTION:
            _tmp_result = OperHandler::subtract(left, right);
            break;
        case ExprKind::MULTIPICATION:
            _tmp_result = OperHandler::multiply(left, right);
            break;
        case ExprKind::DIVISION:
            _tmp_result = OperHandler::divide(left, right);
            break;
        case ExprKind::EQUAL:
            _tmp_result = OperHandler::check_eq(left, right);
            break;
        case ExprKind::NOT_EQUAL:
            _tmp_result = OperHandler::check_neq(left, right);
            break;
        case ExprKind::LESS:
            _tmp_result = OperHandler::check_lt(left, right);
            break;
        case ExprKind::LESS_EQUAL:
            _tmp_result = OperHandler::check_lteq(left, right);
            break;
        case ExprKind::GREATER:
            _tmp_result = OperHandler::check_gt(left, right);
            break;
        case ExprKind::GREATER_EQUAL:
            _tmp_result = OperHandler::check_gteq(left, right);
            break;
        case ExprKind::LOGICAL_AND:
            _tmp_result = OperHandler::logical_and(left, right);
            break;
        case ExprKind::LOGICAL_OR:
            _tmp_result = OperHandler::logical_or(left, right);
            break;
        case ExprKind::FUNCTION_COMPOSITION:
            _tmp_result = OperHandler::compose_functions(left, right);
            break;
        default:
            throw std::logic_error("not implemented");
    }
}

void Interpreter::_evaluate_unary_expr(const ExprKind& expr_kind, value val) {
    if (expr_kind == ExprKind::LOGICAL_NOT) {
        _tmp_result = OperHandler::logical_not(val);
    } else if (expr_kind == ExprKind::UNARY_MINUS) {
        _tmp_result = OperHandler::unary_minus(val);
    } else {
        throw std::logic_error("invalid expr kind passed to evaluate_unary_expr");
    }
}

void Interpreter::_evaluate_condition(const Position& condition_pos) {
    if (not TypeHandler::value_type_is<bool>(_tmp_result)) {
        throw std::runtime_error("condition must be bool type");  // TODO
    }
    _condition_met = TypeHandler::get_value_as<bool>(_tmp_result);
    _clear_tmp_result();
}

bool Interpreter::_tmp_result_is_empty() const {
    return not _tmp_result.has_value();
}

bool Interpreter::_should_exit_code_block() const {
    return _is_returning or _on_break or _on_continue;
}

void Interpreter::_clear_tmp_result() {
    _tmp_result = std::nullopt;
}

void Interpreter::_enter_loop() {
    _inside_loop = true;
    // so the loop variable is visible only within the scope
    _env.add_scope();
}

void Interpreter::_exit_loop() {
    _inside_loop = false;
    _on_continue = false;
    _on_break = false;
    // so the loop var is not visible outside
    _env.pop_scope();
}