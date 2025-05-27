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
