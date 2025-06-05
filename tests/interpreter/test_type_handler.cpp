#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>
#include <tuple>

#include "type_handler.hpp"

std::ostream& operator<<(std::ostream& os, const opt_vhold_or_val& ovv) {
    if (!ovv) {
        os << "none";
    } else {
        std::visit([&os](auto&& arg) { os << arg; }, *ovv);
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::optional<Type>& ret_type) {
    if (ret_type)
        os << ret_type.value().to_str();
    else
        os << "none";
    return os;
}

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

std::vector<std::tuple<value, value, bool>> same_type_cases{
    {value{1}, value{42}, true},
    {value{3.14}, value{2.71}, true},
    {value{std::string("a")}, value{std::string("b")}, true},
    {value{true}, value{false}, true},
    {value{1}, value{3.14}, false},
    {value{1}, value{std::string("a")}, false},
    {value{3.14}, value{true}, false},
    {value{std::string("a")}, value{false}, false},
};

BOOST_DATA_TEST_CASE(are_the_same_type_bdata_test, bdata::make(same_type_cases), lhs, rhs, expected) {
    BOOST_CHECK(TypeHandler::are_the_same_type(lhs, rhs) == expected);
}

std::vector<std::tuple<opt_vhold_or_val, std::optional<Type>, bool>> matches_return_type_cases{
    {std::nullopt, std::nullopt, true},
    {std::nullopt, Type{TypeKind::INT}, false},
    {value{42}, Type{TypeKind::INT}, true},
    {value{42}, Type{TypeKind::FLOAT}, false},
    {value{3.14}, Type{TypeKind::FLOAT}, true},
    {value{3.14}, std::nullopt, false},
    {value{std::string("abc")}, Type{TypeKind::STRING}, true},
    {value{true}, Type{TypeKind::BOOL}, true},
    {value{true}, Type{TypeKind::INT}, false},
};

BOOST_DATA_TEST_CASE(matches_return_type_bdata_test, bdata::make(matches_return_type_cases), ret_val, ret_type,
                     expected) {
    BOOST_CHECK(TypeHandler::matches_return_type(ret_val, ret_type) == expected);
}

std::vector<std::tuple<vhold_or_val, VariableType, bool>> arg_matches_param_cases{
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

BOOST_AUTO_TEST_CASE(value_type_is_for_value) {
    value v_int = 123;
    opt_vhold_or_val val1 = v_int;
    BOOST_CHECK(TypeHandler::value_type_is<int>(val1));
    BOOST_CHECK(!TypeHandler::value_type_is<double>(val1));
    BOOST_CHECK_EQUAL(TypeHandler::get_value_as<int>(val1), 123);

    value v_str = std::string("abc");
    opt_vhold_or_val val2 = v_str;
    BOOST_CHECK(TypeHandler::value_type_is<std::string>(val2));
    BOOST_CHECK(!TypeHandler::value_type_is<int>(val2));
    BOOST_CHECK_EQUAL(TypeHandler::get_value_as<std::string>(val2), "abc");
}

BOOST_AUTO_TEST_CASE(value_type_is_for_variableholder) {
    auto var = std::make_shared<Variable>(VariableType{Type{TypeKind::FLOAT}}, 3.14);
    VariableHolder vh{var};
    opt_vhold_or_val val3 = vh;
    BOOST_CHECK(TypeHandler::value_type_is<double>(val3));
    BOOST_CHECK(!TypeHandler::value_type_is<int>(val3));
    BOOST_CHECK_CLOSE(TypeHandler::get_value_as<double>(val3), 3.14, 1e-9);
}

BOOST_AUTO_TEST_CASE(value_type_is_for_empty) {
    opt_vhold_or_val val4 = std::nullopt;
    BOOST_CHECK(!TypeHandler::value_type_is<int>(val4));
    BOOST_CHECK(!TypeHandler::value_type_is<bool>(val4));
    BOOST_CHECK(!TypeHandler::value_type_is<double>(val4));
    BOOST_CHECK(!TypeHandler::value_type_is<std::string>(val4));
}

std::vector<std::tuple<std::optional<Type>, VariableType, bool>> ret_type_matches_param_type_cases{
    {Type{TypeKind::INT}, VariableType{Type{TypeKind::INT}, false}, true},
    {Type{TypeKind::FLOAT}, VariableType{Type{TypeKind::FLOAT}, false}, true},
    {Type{TypeKind::BOOL}, VariableType{Type{TypeKind::BOOL}, false}, true},
    {Type{TypeKind::STRING}, VariableType{Type{TypeKind::STRING}, false}, true},
    {Type{TypeKind::INT}, VariableType{Type{TypeKind::FLOAT}, false}, false},
    {Type{TypeKind::FLOAT}, VariableType{Type{TypeKind::INT}, false}, false},
    {std::nullopt, VariableType{Type{TypeKind::INT}, false}, false},
    {Type{TypeKind::INT}, VariableType{Type{TypeKind::INT}, true}, true},
    {Type{TypeKind::INT}, VariableType{Type{TypeKind::FLOAT}, true}, false},
    {std::nullopt, VariableType{Type{TypeKind::STRING}, false}, false},
};

BOOST_DATA_TEST_CASE(ret_type_matches_param_type_test, bdata::make(ret_type_matches_param_type_cases), ret_type,
                     param_type, expected) {
    BOOST_CHECK(TypeHandler::ret_type_matches_param_type(ret_type, param_type) == expected);
}

std::vector<std::tuple<value, std::optional<value>>> as_int_cases{
    {value{42}, value{42}},
    {value{3.0}, value{3}},
    {value{true}, value{1}},
    {value{false}, value{0}},
    {value{std::string("123")}, value{123}},
    {value{std::string("abc")}, std::nullopt},
};

BOOST_DATA_TEST_CASE(as_int_test, bdata::make(as_int_cases), input, expected) {
    BOOST_CHECK(TypeHandler::as_int(input) == expected);
}

std::vector<std::tuple<value, std::optional<value>>> as_float_cases{
    {value{42}, value{42.0}},
    {value{3.14}, value{3.14}},
    {value{true}, value{1.0}},
    {value{false}, value{0.0}},
    {value{std::string("2.5")}, value{2.5}},
    {value{std::string("abc")}, std::nullopt},
};

BOOST_DATA_TEST_CASE(as_float_test, bdata::make(as_float_cases), input, expected) {
    BOOST_CHECK(TypeHandler::as_float(input) == expected);
}

std::vector<std::tuple<value, std::optional<value>>> as_bool_cases{
    {value{0}, value{false}},
    {value{1}, value{true}},
    {value{42}, value{true}},
    {value{0.0}, value{false}},
    {value{2.5}, value{true}},
    {value{std::string("true")}, value{true}},
    {value{std::string("")}, value{false}},
    {value{std::string("abc")}, value{true}},
};

BOOST_DATA_TEST_CASE(as_bool_test, bdata::make(as_bool_cases), input, expected) {
    BOOST_CHECK(TypeHandler::as_bool(input) == expected);
}

std::vector<std::tuple<value, std::optional<value>>> as_string_cases{
    {value{42}, value{std::string("42")}},
    {value{3.14}, value{std::string("3.14")}},
    {value{true}, value{std::string("true")}},
    {value{false}, value{std::string("false")}},
    {value{std::string("abc")}, value{std::string("abc")}},
};

BOOST_DATA_TEST_CASE(as_string_test, bdata::make(as_string_cases), input, expected) {
    BOOST_CHECK(TypeHandler::as_string(input) == expected);
}

std::vector<std::tuple<Type, value, std::optional<value>>> as_type_cases{
    {Type{TypeKind::INT}, value{42}, value{42}},
    {Type{TypeKind::INT}, value{3.14}, value{3}},
    {Type{TypeKind::INT}, value{std::string("123")}, value{123}},
    {Type{TypeKind::INT}, value{std::string("abc")}, std::nullopt},

    {Type{TypeKind::FLOAT}, value{42}, value{42.0}},
    {Type{TypeKind::FLOAT}, value{3.14}, value{3.14}},
    {Type{TypeKind::FLOAT}, value{std::string("2.5")}, value{2.5}},
    {Type{TypeKind::FLOAT}, value{std::string("abc")}, std::nullopt},

    {Type{TypeKind::BOOL}, value{0}, value{false}},
    {Type{TypeKind::BOOL}, value{1}, value{true}},
    {Type{TypeKind::BOOL}, value{std::string("true")}, value{true}},
    {Type{TypeKind::BOOL}, value{std::string("")}, value{false}},
    {Type{TypeKind::BOOL}, value{std::string("abc")}, value{true}},

    {Type{TypeKind::STRING}, value{42}, value{std::string("42")}},
    {Type{TypeKind::STRING}, value{3.14}, value{std::string("3.14")}},
    {Type{TypeKind::STRING}, value{true}, value{std::string("true")}},
    {Type{TypeKind::STRING}, value{false}, value{std::string("false")}},
    {Type{TypeKind::STRING}, value{std::string("abc")}, value{std::string("abc")}},
};

BOOST_DATA_TEST_CASE(as_type_test, bdata::make(as_type_cases), type, input, expected) {
    BOOST_CHECK(TypeHandler::as_type(type, input) == expected);
}