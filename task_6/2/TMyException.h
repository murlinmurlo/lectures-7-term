#ifndef TMYEXCEPTION_H
#define TMYEXCEPTION_H

#include <exception>
#include <string>
#include <ostream>

class TMyException : public std::exception {
public:
    TMyException(const std::string& msg);
    const char* what() const noexcept override;

    template<typename T>
    TMyException& operator<<(const T& value);

protected:
    std::string message;
};

class TMyFirstException : public TMyException {
public:
    TMyFirstException(const std::string& msg);
};

class TMySecondException : public TMyException {
public:
    TMySecondException(const std::string& msg);
};

#endif
