#include <print>
#include <string>
#include <generator>
#include <array>
using namespace std::string_literals;

auto list_words() -> std::generator<std::string> {
    auto const words = std::array{
        "C++"s, "is"s, "a"s, "cool"s, "programming"s, "language"s
    };
    for (auto& w : words) co_yield w;
}

int main() {
    for (auto&& w : list_words()) std::print("{} ", w);
}

