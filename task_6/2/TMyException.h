#ifndef TMYEXCEPTION_H
#define TMYEXCEPTION_H

#include <exception>
#include <string>
#include <ostream>

class TMyException : public std::exception {
private:
    std::string message;

public:
    explicit TMyException(const std::string& msg);
    virtual const char* what() const noexcept override;

    friend std::ostream& operator<<(std::ostream& os, const TMyException& ex);
};

class TMyFirstException : public TMyException {
public:
    explicit TMyFirstException(const std::string& msg);
};

class TMySecondException : public TMyException {
public:
    explicit TMySecondException(const std::string& msg);
};

#endif
