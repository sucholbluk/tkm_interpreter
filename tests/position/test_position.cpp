#define BOOST_TEST_MODULE Position_UnitTests
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/included/unit_test.hpp>

#include "position.hpp"

BOOST_AUTO_TEST_CASE(default_constructor_test) {
    Position position;
    BOOST_CHECK_EQUAL(position.get_line(), 1);
    BOOST_CHECK_EQUAL(position.get_column(), 1);
}

namespace bdata = boost::unit_test::data;

std::vector<int> next_method_test_cases = {4, 5, 2, 90, 999, 435, 100, 123};

BOOST_DATA_TEST_CASE(next_column_test, bdata::make(next_method_test_cases), iterations) {
    Position position;
    for (int i = 1; i <= iterations; i++) {
        position.next_column();
        BOOST_CHECK_EQUAL(position.get_column(), i + 1);
        BOOST_CHECK_EQUAL(position.get_line(), 1);
    }
}

BOOST_DATA_TEST_CASE(next_line_test, bdata::make(next_method_test_cases), iterations) {
    Position position;
    for (int i = 1; i <= iterations; i++) {
        position.next_line();
        BOOST_CHECK_EQUAL(position.get_column(), 1);
        BOOST_CHECK_EQUAL(position.get_line(), i + 1);
    }
}

BOOST_DATA_TEST_CASE(next_line_and_col_test, bdata::make(next_method_test_cases), iterations) {
    Position position;
    for (int i = 1; i <= iterations; i++) {
        for (int j = 1; j <= iterations; j++) {
            position.next_column();
            BOOST_CHECK_EQUAL(position.get_column(), j + 1);
        }
        position.next_line();
        BOOST_CHECK_EQUAL(position.get_column(), 1);
        BOOST_CHECK_EQUAL(position.get_line(), i + 1);
    }
}
