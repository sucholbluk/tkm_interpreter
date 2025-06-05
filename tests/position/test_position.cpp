#define BOOST_TEST_MODULE POSITON_TESTS
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>

#include "position.hpp"

BOOST_AUTO_TEST_CASE(default_constructor_test) {
    Position position;
    BOOST_CHECK_EQUAL(position.get_line(), 1);
    BOOST_CHECK_EQUAL(position.get_column(), 1);
}

namespace bdata = boost::unit_test::data;

std::vector<int> next_method_test_cases{4, 5, 2, 90, 999, 435, 100, 123};

BOOST_DATA_TEST_CASE(next_column_test, bdata::make(next_method_test_cases), iterations) {
    Position position;
    for (int i = 1; i <= iterations; i++) {
        position = position.next_column();
        BOOST_CHECK_EQUAL(position.get_column(), i + 1);
        BOOST_CHECK_EQUAL(position.get_line(), 1);
    }
}

BOOST_DATA_TEST_CASE(next_line_test, bdata::make(next_method_test_cases), iterations) {
    Position position;
    for (int i = 1; i <= iterations; i++) {
        position = position.next_line();
        BOOST_CHECK_EQUAL(position.get_column(), 1);
        BOOST_CHECK_EQUAL(position.get_line(), i + 1);
    }
}

BOOST_DATA_TEST_CASE(next_line_and_col_test, bdata::make(next_method_test_cases), iterations) {
    Position position;
    for (int i = 1; i <= iterations; i++) {
        for (int j = 1; j <= iterations; j++) {
            position = position.next_column();
            BOOST_CHECK_EQUAL(position.get_column(), j + 1);
        }
        position = position.next_line();
        BOOST_CHECK_EQUAL(position.get_column(), 1);
        BOOST_CHECK_EQUAL(position.get_line(), i + 1);
    }
}

std::vector<std::tuple<Position, Position>> equal_test_cases{
    {Position{}, Position{}},
    {Position{80, 78}, Position{80, 78}},
    {Position{10, 20}, Position{10, 20}},
    {Position{100, 200}, Position{100, 200}},
    {Position{50, 50}, Position{50, 50}},
    {Position{999, 999}, Position{999, 999}},
};

BOOST_DATA_TEST_CASE(equality_operator_test, bdata::make(equal_test_cases), position1, position2) {
    BOOST_CHECK_EQUAL(position1, position2);
}

std::vector<std::tuple<Position, Position>> not_equal_test_cases{
    {Position{}, Position{9, 9}},         {Position{84, 78}, Position{80, 78}},
    {Position{11, 20}, Position{10, 20}}, {Position{190, 200}, Position{100, 200}},
    {Position{50, 51}, Position{50, 50}}, {Position{999, 1}, Position{999, 999}},
};

BOOST_DATA_TEST_CASE(inequality_operator_test, bdata::make(not_equal_test_cases), position1, position2) {
    BOOST_CHECK_NE(position1, position2);
}

std::vector<std::tuple<Position, std::string>> print_test_cases{
    {Position{}, "[1:1]"},           {Position{4, 9}, "[4:9]"},
    {Position{10, 20}, "[10:20]"},   {Position{100, 200}, "[100:200]"},
    {Position{50, 50}, "[50:50]"},   {Position{999, 999}, "[999:999]"},
    {Position{1, 1000}, "[1:1000]"}, {Position{123, 456}, "[123:456]"},
    {Position{19, 4}, "[19:4]"},     {Position{500, 1}, "[500:1]"},
};

BOOST_DATA_TEST_CASE(print_test, bdata::make(print_test_cases), position, expected_output) {
    BOOST_CHECK_EQUAL(position.get_position_str(), expected_output);
}
