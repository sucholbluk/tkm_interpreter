#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>
#include <tuple>

#include "type_handler.hpp"
#include "values.h"

namespace bdata = boost::unit_test::data;

std::vector<std::tuple<Type, value>> deduce_type_test_cases{
    {Type{TypeKind::INT}, 42},
    {Type{TypeKind::FLOAT}, 3.14},
    {Type{TypeKind::BOOL}, true},
    {Type{TypeKind::STRING}, std::string("hello")},
};

BOOST_DATA_TEST_CASE(deduce_type_test, bdata::make(deduce_type_test_cases), expected_type, val) {
    BOOST_CHECK(expected_type == TypeHandler::deduce_type(val));
}

#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>
#include <tuple>

#include "type_handler.hpp"
#include "variable.hpp"

namespace bdata = boost::unit_test::data;

std::vector<std::tuple<arg, VariableType, bool>> arg_matches_param_cases{
    {value{42}, VariableType{Type{TypeKind::INT}, false}, true},
    {value{3.14}, VariableType{Type{TypeKind::FLOAT}, false}, true},
    {value{true}, VariableType{Type{TypeKind::BOOL}, false}, true},
    {value{std::string("abc")}, VariableType{Type{TypeKind::STRING}, false}, true},
    {value{42}, VariableType{Type{TypeKind::FLOAT}, false}, false},
    {value{std::string("abc")}, VariableType{Type{TypeKind::INT}, false}, false},
};

BOOST_DATA_TEST_CASE(arg_matches_param_test, bdata::make(arg_matches_param_cases), a, param, expected) {
    BOOST_CHECK(TypeHandler::arg_matches_param(a, param) == expected);
}

std::vector<std::tuple<arg_list, std::vector<VariableType>, bool>> args_match_params_cases{
    {{value{42}, value{std::string("abc")}},
     {VariableType{Type{TypeKind::INT}, false}, VariableType{Type{TypeKind::STRING}, false}},
     true},
    {{value{42}}, {VariableType{Type{TypeKind::INT}, false}, VariableType{Type{TypeKind::STRING}, false}}, false},
    {{value{42}, value{3.14}},
     {VariableType{Type{TypeKind::INT}, false}, VariableType{Type{TypeKind::FLOAT}, false}},
     true},
    {{value{42}, value{3.14}},
     {VariableType{Type{TypeKind::INT}, false}, VariableType{Type{TypeKind::STRING}, false}},
     false}};

BOOST_DATA_TEST_CASE(args_match_params_test, bdata::make(args_match_params_cases), args, params, expected) {
    BOOST_CHECK(TypeHandler::args_match_params(args, params) == expected);
}

BOOST_AUTO_TEST_CASE(mut_var_match_type_test) {
    // mutable variable visible through holder
    auto var_hold = VariableHolder{std::make_shared<Variable>(VariableType{TypeHandler::deduce_type(9), true}, 9)};

    // should be good for mutable param type
    BOOST_CHECK(TypeHandler::arg_matches_param(var_hold, VariableType{Type{TypeKind::INT}, true}));
    // but also for immutable param
    BOOST_CHECK(TypeHandler::arg_matches_param(var_hold, VariableType{Type{TypeKind::INT}, false}));
}

BOOST_AUTO_TEST_CASE(var_match_type_test) {
    // immutable variable visible through holder
    auto var_hold =
        VariableHolder{std::make_shared<Variable>(VariableType{TypeHandler::deduce_type("Hello"), false}, "Hello")};

    // should be good for immutable param type
    BOOST_CHECK(TypeHandler::arg_matches_param(var_hold, VariableType{Type{TypeKind::STRING}, false}));
    // should be invalid for mutable param type - variables are passed by reference
    BOOST_CHECK(not TypeHandler::arg_matches_param(var_hold, VariableType{Type{TypeKind::STRING}, true}));
}
