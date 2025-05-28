#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>

#include "scope.hpp"
#include "type_handler.hpp"
#include "variable.hpp"

BOOST_AUTO_TEST_CASE(add_and_get_variable_test) {
    Scope scope;
    auto var_h = VariableHolder{std::make_shared<Variable>(VariableType{TypeHandler::deduce_type(4)}, 4)};
    scope.add_variable("x", var_h);

    auto result = scope.get_variable("x");
    BOOST_CHECK(result.has_value() and result.value().var == var_h.var);
}

BOOST_AUTO_TEST_CASE(variable_not_found_test) {
    Scope scope;
    auto result = scope.get_variable("y");
    BOOST_CHECK(not result);
}

BOOST_AUTO_TEST_CASE(contains_variable_test) {
    Scope scope;
    auto var = std::make_shared<Variable>(VariableType{TypeHandler::deduce_type(0.12)}, 0.12);
    scope.add_variable("z", var);

    BOOST_CHECK(scope.contains_variable("z"));
    BOOST_CHECK(!scope.contains_variable("not_exists"));
}

BOOST_AUTO_TEST_CASE(modify_variable_test) {
    Scope scope;
    auto var = std::make_shared<Variable>(VariableType{TypeHandler::deduce_type(4)}, 4);
    scope.add_variable("x", var);

    var->var_value = 8;
    auto result = scope.get_variable("x");
    BOOST_CHECK(result and result.value().get_value_as<int>() == 8);
}

BOOST_AUTO_TEST_CASE(modify_variable_after_get_test) {
    Scope scope;
    auto var = std::make_shared<Variable>(VariableType{TypeHandler::deduce_type("Hello")}, "Hello");
    scope.add_variable("x", var);

    auto result = scope.get_variable("x");
    BOOST_CHECK(result);
    result.value().var->var_value = "Goodbye";
    BOOST_CHECK(get_value_as<std::string>(var->var_value) == "Goodbye");
}