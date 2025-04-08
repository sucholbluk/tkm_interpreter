#ifndef CONSTANTS_HPP

constexpr char EOF_CHAR{'\x03'};
constexpr char LF_CHAR{'\n'};
constexpr char CR_CHAR{'\r'};
constexpr int MAX_IDENTIFIER_LEN{256};  // just for now - to be parametrized later with flags
constexpr int MAX_COMMENT_LEN{512};

#define CONSTANTS_HPP
#endif  // CONSTANTS_HPP