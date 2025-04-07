#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#endif  // EXCEPTIONS_HPP
#include <stdexcept>
#include <string>

class InvalidTokenValueException : public std::logic_error {
   public:
    explicit InvalidTokenValueException(const std::string& message)
        : std::logic_error("Invalid token value: " + message) {}
};

class InvalidGetTokenValueException : public std::logic_error {
   public:
    explicit InvalidGetTokenValueException(const std::string& message)
        : std::logic_error("Invalid get_value in token: " + message) {}
};

class UnexpectedCharacterException : public std::runtime_error {
   public:
    explicit UnexpectedCharacterException(const std::string& message)
        : std::runtime_error("Unexpected char at position: " + message) {}
};

class IntValueOverflowException : public std::runtime_error {
   public:
    explicit IntValueOverflowException(const std::string& message)
        : std::runtime_error("Value too largeat : " + message) {}
};