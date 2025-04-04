#define BOOST_TEST_MODULE SourceHandler_UnitTests

#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <tuple>

#include "source_handler.hpp"

namespace bdata = boost::unit_test::data;

std::vector<std::tuple<std::string, std::string>> read_values_test_cases = {
    {"Hello\nWorld!", "Hello\nWorld!"},
    {"Shorten carriage\r\nNewline", "Shorten carriage\nNewline"},
    {"Carriage at the end\r\n", "Carriage at the end\n"},
    {"Another\n test\n!@#$%^&*()\"", "Another\n test\n!@#$%^&*()\""},
};

BOOST_DATA_TEST_CASE(read_values_test, bdata::make(read_values_test_cases), input_str, expected_output) {
    std::stringstream source{input_str};

    SourceHandler handler{source};
    std::string handler_output{};
    char c;
    while ((c = handler.get_char_and_position().first) != EOF_CHAR) {
        handler_output += c;
    }

    BOOST_CHECK_EQUAL(handler_output, expected_output);
}

std::vector<std::tuple<std::string, Position>> eof_position_test_cases{
    {"", Position{1, 1}},
    {"\n\n\n", Position{4, 1}},
    {"Hello world!", Position{1, 13}},
    {"Line1\nLine2\nLine3", Position{3, 6}},
    {"\nHello\nWorld\n", Position{4, 1}},
    {"1234567890", Position{1, 11}},
    {"\r\n\r\n", Position{3, 1}},
    {"\n\n\n\n\n", Position{6, 1}},
    {"Test\nwith\nmultiple\nlines\nand\nEOF", Position{6, 4}},
    {"\n\nEnd", Position{3, 4}},
    {"Mixed\r\nLine\nEndings\r\nHere", Position{4, 5}},
};

BOOST_DATA_TEST_CASE(eof_position_test, bdata::make(eof_position_test_cases), input_str, eof_position) {
    std::stringstream source{input_str};
    std::pair<char, Position> char_pos;
    SourceHandler handler{source};
    do {
        char_pos = handler.get_char_and_position();
    } while (char_pos.first != EOF_CHAR);

    BOOST_CHECK_EQUAL(char_pos.second, eof_position);
}

std::vector<std::tuple<std::string, Position, int>> requests_after_eof_test_cases{
    {"", Position{1, 1}, 1},
    {"\n\n\n", Position{4, 1}, 10},
    {"Hello world!", Position{1, 13}, 90},
    {"Line1\nLine2\nLine3", Position{3, 6}, 4},
    {"\nHello\nWorld\n", Position{4, 1}, 17},
    {"1234567890", Position{1, 11}, 15},
    {"\r\n\r\n", Position{3, 1}, 219},
    {"\n\n\n\n\n", Position{6, 1}, 999},
    {"Test\nwith\nmultiple\nlines\nand\nEOF", Position{6, 4}, 3},
    {"\n\nEnd", Position{3, 4}, 84},
    {"Mixed\r\nLine\nEndings\r\nHere", Position{4, 5}, 55},
};

BOOST_DATA_TEST_CASE(request_chars_after_eof_test, bdata::make(requests_after_eof_test_cases), input_str, eof_position, n_times) {
    std::stringstream source{input_str};
    std::pair<char, Position> char_pos;
    SourceHandler handler{source};
    do {
        char_pos = handler.get_char_and_position();
    } while (char_pos.first != EOF_CHAR);

    for (int i = 0; i < n_times; i++) {
        char_pos = handler.get_char_and_position();
    }

    BOOST_CHECK_EQUAL(char_pos.second, eof_position);
}
