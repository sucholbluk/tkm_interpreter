#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>

#include "call_frame.hpp"
#include "type_handler.hpp"

BOOST_AUTO_TEST_CASE(add_and_find_variable_test) {
    CallFrame frame;
    auto var = std::make_shared<Variable>(VariableType{TypeHandler::deduce_type(123)}, 123);
    frame.add_variable("x", var);

    auto found = frame.find_variable("x");
    BOOST_CHECK(found == var);
}

BOOST_AUTO_TEST_CASE(find_variable_in_inner_scope_test) {
    CallFrame frame;
    auto var_outer = std::make_shared<Variable>(VariableType{TypeHandler::deduce_type(1)}, 1);
    frame.add_variable("a", var_outer);

    frame.push_scope();
    auto var_inner = std::make_shared<Variable>(VariableType{TypeHandler::deduce_type(2)}, 2);
    frame.add_variable("b", var_inner);

    BOOST_CHECK(frame.find_variable("a") == var_outer);
    BOOST_CHECK(frame.find_variable("b") == var_inner);

    frame.pop_scope();
    BOOST_CHECK(frame.find_variable("b") == nullptr);
    BOOST_CHECK(frame.find_variable("a") == var_outer);
}

BOOST_AUTO_TEST_CASE(is_in_current_scope_test) {
    CallFrame frame;
    auto var = std::make_shared<Variable>(VariableType{TypeHandler::deduce_type(42)}, 42);
    frame.add_variable("foo", var);

    BOOST_CHECK(frame.is_in_current_scope("foo"));
    BOOST_CHECK(!frame.is_in_current_scope("bar"));

    frame.push_scope();
    BOOST_CHECK(!frame.is_in_current_scope("foo"));
}

BOOST_AUTO_TEST_CASE(modify_variable_value_test) {
    CallFrame frame;
    auto var = std::make_shared<Variable>(VariableType{TypeHandler::deduce_type(7)}, 7);
    frame.add_variable("num", var);

    // Modyfikacja wartości
    var->var_value = 99;
    auto found = frame.find_variable("num");
    BOOST_REQUIRE(found != nullptr);
    BOOST_CHECK(std::get<int>(found->var_value) == 99);

    // shadowing
    frame.push_scope();
    auto var2 = std::make_shared<Variable>(VariableType{TypeHandler::deduce_type(5)}, 5);
    frame.add_variable("num", var2);
    var2->var_value = 123;
    auto found2 = frame.find_variable("num");
    BOOST_REQUIRE(found2 != nullptr);
    BOOST_CHECK(std::get<int>(found2->var_value) == 123);

    frame.pop_scope();
    auto found3 = frame.find_variable("num");
    BOOST_REQUIRE(found3 != nullptr);
    BOOST_CHECK(std::get<int>(found3->var_value) == 99);
}
