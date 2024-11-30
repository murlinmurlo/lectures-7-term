#include <iostream>
#include <gtest/gtest.h>
#include "TMyException.h"

TMyException::TMyException(const std::string& msg) : message(msg) {}

const char* TMyException::what() const noexcept {
    return message.c_str();
}

template<typename T>
TMyException& TMyException::operator<<(const T& value) {
    std::ostringstream oss;
    oss << value;
    message += oss.str();
    return *this;
}

TMyFirstException::TMyFirstException(const std::string& msg) 
    : TMyException(msg) {}

TMySecondException::TMySecondException(const std::string& msg) 
    : TMyException(msg) {}

TEST(MyExceptionTest, FirstExceptionTest) {
    int value = 42;
    TMyFirstException ex("error: ");
    ex << "invalid value:" << value;

    EXPECT_STREQ(ex.what(), "error: invalid value:42");
}

TEST(MyExceptionTest, SecondExceptionTest) {
    TMySecondException ex("error: ");
    ex << "some additional info";

    EXPECT_STREQ(ex.what(), "error: some additional info");
}

TEST(MyExceptionTest, DoubleValueTest) {
    double value = 1.23;
    TMyFirstException ex("error: ");
    ex << "invalid value:" << value;

    EXPECT_STREQ(ex.what(), "error: invalid value:1.23");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
