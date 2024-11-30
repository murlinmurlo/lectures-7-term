#include <iostream>
#include <gtest/gtest.h>
#include "TMyException.h"

TMyException::TMyException(const std::string& msg) : message(msg) {}

const char* TMyException::what() const noexcept {
    return message.c_str();
}

std::ostream& operator<<(std::ostream& os, const TMyException& ex) {
    os << ex.message;
    return os;
}

TMyException& TMyException::operator<<(const std::string& msg) {
    message += " " + msg;
    return *this;
}

TMyException& TMyException::operator<<(int value) {
    message += " " + std::to_string(value);
    return *this;
}

TMyFirstException::TMyFirstException(const std::string& msg) 
    : TMyException(msg) {}

TMySecondException::TMySecondException(const std::string& msg) 
    : TMyException(msg) {}

TEST(MyExceptionTest, FirstExceptionTest) {
    int value = 42;
    TMyFirstException ex("error:");
    ex << "invalid value:" << value;
    
    EXPECT_STREQ(ex.what(), "error: invalid value: 42");
}

TEST(MyExceptionTest, SecondExceptionTest) {
    TMySecondException ex("error:");
    ex << "some additional info";
    
    EXPECT_STREQ(ex.what(), "error: some additional info");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
