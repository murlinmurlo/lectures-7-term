#include <type_traits>
#include <iostream>
#include <array>

template <char ...c>
struct TString {
    static constexpr std::array<char, sizeof...(c) + 1> chars = {c..., '\0'};
};

template <char ...c>
constexpr std::array<char, sizeof...(c) + 1> TString<c...>::chars;

template <char ...c1, char ...c2>
constexpr auto operator==(TString<c1...>, TString<c2...>) {
    return std::is_same<TString<c1...>, TString<c2...>>{};
}

template <char ...c1, char ...c2>
constexpr auto operator+(TString<c1...>, TString<c2...>) {
    return TString<c1..., c2...>{};
}

template <typename CharT, CharT ...c>
constexpr TString<c...> operator"" _s() {
    return {};
}

template <char ...c>
std::ostream& operator<<(std::ostream& os, const TString<c...>&) {
    return os << TString<c...>::chars.data();
}

int main() {
    constexpr auto hello = "hello"_s + " world"_s;
    std::cout << hello << std::endl;
    static_assert(hello == "hello world"_s);
    return 0;
}
