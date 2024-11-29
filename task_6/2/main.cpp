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

TMyFirstException::TMyFirstException(const std::string& msg) 
    : TMyException("exception 1: " + msg) {}

TMySecondException::TMySecondException(const std::string& msg) 
    : TMyException("exception 2: " + msg) {}


TEST(MyExceptionTest, FirstExceptionTest) {
    EXPECT_THROW({
        throw TMyFirstException("error");
    }, TMyFirstException);
}

TEST(MyExceptionTest, SecondExceptionTest) {
    EXPECT_THROW({
        throw TMySecondException("error");
    }, TMySecondException);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
