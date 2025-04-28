#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>
#include <tuple>

#include "type.hpp"

namespace bdata = boost::unit_test::data;

/* -----------------------------------------------------------------------------*
 *                                      TYPE                                    *
 *------------------------------------------------------------------------------*/

BOOST_AUTO_TEST_SUITE(type_tests)
std::vector<TypeKind> type_kinds = {
    TypeKind::INT,
    TypeKind::FLOAT,
    TypeKind::STRING,
    TypeKind::BOOL,
};

BOOST_DATA_TEST_CASE(constructor_with_type_kind_test, bdata::make(type_kinds), kind) {
    Type int_type{kind};
    BOOST_CHECK_EQUAL(int_type.kind, kind);
    BOOST_CHECK(int_type.function_type_info == nullptr);
}

BOOST_AUTO_TEST_CASE(constructor_with_function_kind_test) {
    BOOST_CHECK_THROW(Type{TypeKind::FUNCTION}, std::invalid_argument);  // TODO: replace with custom except after impl
}

std::vector<FunctionTypeInfo> function_type_infos = {
    // function<none:none>
    FunctionTypeInfo{{}},
    // function<mut int,int:none>
    FunctionTypeInfo{
        {
            ParamType{Type{TypeKind::INT}, true},
            ParamType{Type{TypeKind::INT}},
        }},
    // function<none:int>
    FunctionTypeInfo{{}, Type{TypeKind::INT}},
    // function<int,bool:string>
    FunctionTypeInfo{
        {
            ParamType{Type{TypeKind::INT}},
            ParamType{Type{TypeKind::BOOL}},
        },
        Type{TypeKind::STRING}},
    // function<float:bool>
    FunctionTypeInfo{
        {
            ParamType{Type{TypeKind::FLOAT}},
        },
        Type{TypeKind::BOOL}},
    // function<mut string:none>
    FunctionTypeInfo{
        {
            ParamType{Type{TypeKind::STRING}, true},
        }},
    // function<int,mut float:float>
    FunctionTypeInfo{
        {
            ParamType{Type{TypeKind::INT}},
            ParamType{Type{TypeKind::FLOAT}, true},
        },
        Type{TypeKind::FLOAT}},
    // function<function<mut float:int>:none>
    FunctionTypeInfo{
        {
            ParamType{Type{
                FunctionTypeInfo{
                    {ParamType{Type{TypeKind::FLOAT}, true}},
                    Type{TypeKind::INT}}}},
        }},

    // function<none:function<int:int>>
    FunctionTypeInfo{
        {},
        Type{
            FunctionTypeInfo{
                {ParamType{Type{TypeKind::INT}}},
                Type{TypeKind::INT}}}},

    // function<function<string:int>:function<int:int>>
    FunctionTypeInfo{
        {
            ParamType{Type{
                FunctionTypeInfo{
                    {ParamType{Type{TypeKind::STRING}}},
                    Type{TypeKind::INT}}}},
        },
        Type{
            FunctionTypeInfo{
                {ParamType{Type{TypeKind::INT}}},
                Type{TypeKind::INT}}}},

    // function<function<int:int>,function<float:bool>:function<string:none>>
    FunctionTypeInfo{
        {
            ParamType{Type{
                FunctionTypeInfo{
                    {ParamType{Type{TypeKind::INT}}},
                    Type{TypeKind::INT}}}},
            ParamType{Type{
                FunctionTypeInfo{
                    {ParamType{Type{TypeKind::FLOAT}}},
                    Type{TypeKind::BOOL}}}},
        },
        Type{
            FunctionTypeInfo{
                {ParamType{Type{TypeKind::STRING}}},
                std::nullopt}}},
};

BOOST_DATA_TEST_CASE(constructor_with_function_type_info_test, bdata::make(function_type_infos), fun_type_info) {
    Type func_type{fun_type_info};
    BOOST_CHECK_EQUAL(func_type.kind, TypeKind::FUNCTION);
    BOOST_CHECK(func_type.function_type_info != nullptr);
}

BOOST_AUTO_TEST_CASE(function_returning_function_test) {
    FunctionTypeInfo inner_function{
        {ParamType{Type{TypeKind::INT}}},
        Type{TypeKind::INT}};

    // function<function<int:int>:none>
    FunctionTypeInfo outer_function{
        {ParamType{Type{inner_function}}},
        std::nullopt};

    Type outer_function_type(outer_function);

    BOOST_CHECK_EQUAL(outer_function_type.kind, TypeKind::FUNCTION);
    BOOST_CHECK(outer_function_type.function_type_info != nullptr);
    BOOST_CHECK_EQUAL(outer_function_type.function_type_info->param_types.size(), 1);
    BOOST_CHECK_EQUAL(outer_function_type.function_type_info->param_types[0].type.kind, TypeKind::FUNCTION);
}

std::vector<std::tuple<Type, std::string>> types_and_strs = {
    {Type{type_kinds[0]}, "int"},
    {Type{type_kinds[1]}, "float"},
    {Type{type_kinds[2]}, "string"},
    {Type{type_kinds[3]}, "bool"},
    {Type{function_type_infos[0]}, "function<none:none>"},
    {Type{function_type_infos[1]}, "function<mut int,int:none>"},
    {Type{function_type_infos[2]}, "function<none:int>"},
    {Type{function_type_infos[3]}, "function<int,bool:string>"},
    {Type{function_type_infos[4]}, "function<float:bool>"},
    {Type{function_type_infos[5]}, "function<mut string:none>"},
    {Type{function_type_infos[6]}, "function<int,mut float:float>"},
    {Type{function_type_infos[7]}, "function<function<mut float:int>:none>"},
    {Type{function_type_infos[8]}, "function<none:function<int:int>>"},
    {Type{function_type_infos[9]}, "function<function<string:int>:function<int:int>>"},
    {Type{function_type_infos[10]}, "function<function<int:int>,function<float:bool>:function<string:none>>"},
};

BOOST_DATA_TEST_CASE(type_to_str_test, bdata::make(types_and_strs), type, expected_str) {
    BOOST_CHECK_EQUAL(type.to_str(), expected_str);
}

BOOST_AUTO_TEST_CASE(equality_operators_test) {
    Type int_type1(TypeKind::INT);
    Type int_type2(TypeKind::INT);
    BOOST_CHECK(int_type1 == int_type2);

    FunctionTypeInfo func_info{{}, std::nullopt};
    Type func_type1(func_info);
    Type func_type2(func_info);
    BOOST_CHECK(func_type1 == func_type2);

    BOOST_CHECK(int_type1 != func_type1);
}

BOOST_AUTO_TEST_SUITE_END()

/* -----------------------------------------------------------------------------*
 *                                 PARAM_TYPE                                   *
 *------------------------------------------------------------------------------*/

BOOST_AUTO_TEST_SUITE(param_type_tests)

BOOST_AUTO_TEST_CASE(constructor_test) {
    Type int_type(TypeKind::INT);
    ParamType param(int_type, true);
    BOOST_CHECK(param.type == int_type);
    BOOST_CHECK(param.is_mutable);
}

BOOST_AUTO_TEST_CASE(to_str_test) {
    Type int_type(TypeKind::INT);
    ParamType param(int_type, true);
    BOOST_CHECK_EQUAL(param.to_str(), "mut int");

    ParamType param2(int_type, false);
    BOOST_CHECK_EQUAL(param2.to_str(), "int");
}

BOOST_AUTO_TEST_CASE(equality_operators_test) {
    Type int_type(TypeKind::INT);
    ParamType param1(int_type, true);
    ParamType param2(int_type, true);
    BOOST_CHECK(param1 == param2);

    ParamType param3(int_type, false);
    BOOST_CHECK(param1 != param3);
}

BOOST_AUTO_TEST_SUITE_END()

/* -----------------------------------------------------------------------------*
 *                             FUNCTION_TYPE_INFO                               *
 *------------------------------------------------------------------------------*/

BOOST_AUTO_TEST_SUITE(function_type_info_tests)

BOOST_AUTO_TEST_CASE(constructor_test) {
    Type int_type(TypeKind::INT);
    ParamType param(int_type, true);
    FunctionTypeInfo func_info{{param}, int_type};

    BOOST_CHECK_EQUAL(func_info.param_types.size(), 1);
    BOOST_CHECK(func_info.return_type.has_value());
    BOOST_CHECK(func_info.return_type.value() == int_type);
}

BOOST_AUTO_TEST_CASE(to_str_test) {
    Type int_type(TypeKind::INT);
    ParamType param(int_type, true);
    FunctionTypeInfo func_info{{param}, int_type};

    BOOST_CHECK_EQUAL(func_info.to_str(), "<mut int:int>");

    FunctionTypeInfo func_info2{{}};
    BOOST_CHECK_EQUAL(func_info2.to_str(), "<none:none>");
}

BOOST_AUTO_TEST_CASE(equality_operators_test) {
    Type int_type(TypeKind::INT);
    ParamType param(int_type, true);
    FunctionTypeInfo func_info1{{param}, int_type};
    FunctionTypeInfo func_info2{{param}, int_type};

    BOOST_CHECK(func_info1 == func_info2);

    FunctionTypeInfo func_info3{{}, std::nullopt};
    BOOST_CHECK(func_info1 != func_info3);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(type_kind_tests)

BOOST_AUTO_TEST_CASE(type_kind_to_string_test) {
    BOOST_CHECK_EQUAL(type_kind_to_string(TypeKind::INT), "int");
    BOOST_CHECK_EQUAL(type_kind_to_string(TypeKind::FLOAT), "float");
    BOOST_CHECK_EQUAL(type_kind_to_string(TypeKind::BOOL), "bool");
    BOOST_CHECK_EQUAL(type_kind_to_string(TypeKind::STRING), "string");
    BOOST_CHECK_EQUAL(type_kind_to_string(TypeKind::FUNCTION), "function");
}

BOOST_AUTO_TEST_SUITE_END()