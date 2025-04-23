#define BOOST_TEST_MODULE TYPE_TESTS
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/included/unit_test.hpp>

#include "type.hpp"

BOOST_AUTO_TEST_CASE(test_basic_type_to_string) {
    BOOST_CHECK_EQUAL(type_to_string(BasicType::INT), "int");
    BOOST_CHECK_EQUAL(type_to_string(BasicType::FLOAT), "float");
    BOOST_CHECK_EQUAL(type_to_string(BasicType::STRING), "string");
    BOOST_CHECK_EQUAL(type_to_string(BasicType::BOOL), "bool");
    BOOST_CHECK_EQUAL(type_to_string(BasicType::NONE), "none");
}

BOOST_AUTO_TEST_CASE(test_function_type_to_string) {
    auto func_type = std::make_shared<FunctionType>();
    func_type->param_types.push_back(BasicType::FLOAT);
    func_type->param_types.push_back(BasicType::STRING);
    func_type->return_type = BasicType::BOOL;

    std::string expected = "function<float,string:bool>";

    BOOST_CHECK_EQUAL(type_to_string(func_type), expected);
}

BOOST_AUTO_TEST_CASE(test_nested_function_type_to_string) {
    auto inner_func_type = std::make_shared<FunctionType>();
    inner_func_type->param_types.push_back(BasicType::INT);
    inner_func_type->return_type = BasicType::FLOAT;

    auto outer_func_type = std::make_shared<FunctionType>();
    outer_func_type->param_types.push_back(inner_func_type);
    outer_func_type->return_type = BasicType::STRING;

    std::string expected = "function<function<int:float>:string>";

    BOOST_CHECK_EQUAL(type_to_string(outer_func_type), expected);
}

BOOST_AUTO_TEST_CASE(test_basic_type_comparison) {
    BOOST_CHECK(BasicType::INT == BasicType::INT);
    BOOST_CHECK(BasicType::FLOAT != BasicType::INT);
    BOOST_CHECK(BasicType::BOOL != BasicType::STRING);
}

BOOST_AUTO_TEST_CASE(test_function_type_comparison) {
    auto func_type1 = std::make_shared<FunctionType>();
    func_type1->param_types.push_back(BasicType::FLOAT);
    func_type1->param_types.push_back(BasicType::STRING);
    func_type1->return_type = BasicType::BOOL;

    auto func_type2 = std::make_shared<FunctionType>();
    func_type2->param_types.push_back(BasicType::FLOAT);
    func_type2->param_types.push_back(BasicType::STRING);
    func_type2->return_type = BasicType::BOOL;

    auto func_type3 = std::make_shared<FunctionType>();
    func_type3->param_types.push_back(BasicType::INT);
    func_type3->return_type = BasicType::FLOAT;

    BOOST_CHECK(*func_type1 == *func_type2);
    BOOST_CHECK(*func_type1 != *func_type3);
}

BOOST_AUTO_TEST_CASE(test_type_comparison) {
    Type type1 = BasicType::INT;
    Type type2 = BasicType::INT;
    Type type3 = BasicType::FLOAT;

    BOOST_CHECK(type1 == type2);
    BOOST_CHECK(type1 != type3);

    auto func_type1 = std::make_shared<FunctionType>();
    func_type1->param_types.push_back(BasicType::FLOAT);
    func_type1->return_type = BasicType::BOOL;

    auto func_type2 = std::make_shared<FunctionType>();
    func_type2->param_types.push_back(BasicType::FLOAT);
    func_type2->return_type = BasicType::BOOL;

    auto func_type3 = std::make_shared<FunctionType>();
    func_type3->param_types.push_back(BasicType::INT);
    func_type3->return_type = BasicType::FLOAT;

    Type type4 = func_type1;
    Type type5 = func_type2;
    Type type6 = func_type3;

    BOOST_CHECK(type4 == type5);
    BOOST_CHECK(type4 != type6);
    BOOST_CHECK(type1 != type4);
}