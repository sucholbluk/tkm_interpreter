#ifndef CONSTANTS_HPP

constexpr char EOF_CHAR{'\x03'};
constexpr char LF_CHAR{'\n'};
constexpr char CR_CHAR{'\r'};
// just for now - to be parametrized later with flags
constexpr int MAX_IDENTIFIER_LEN{256};
constexpr int MAX_COMMENT_LEN{512};
constexpr int MAX_STR_LITERAL_LEN{1024};

#define CONSTANTS_HPP
#endif  // CONSTANTS_HPP