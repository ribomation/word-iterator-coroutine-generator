#include <print>
#include <string>
#include <generator>
#include <array>
using namespace std::string_literals;

auto ListWord() -> std::generator<std::string> {
    auto const words = std::array{
        "C++"s, "is"s, "cool"s
    };
    for (auto& w : words) co_yield w;
}

void use_iterator() {
    std::print("iter: ");
    auto coro  = ListWord();
    auto first = coro.begin();
    auto last  = coro.end();
    while (first != last) {
        auto&& word = *first;
        std::print("{} ", word);
        ++first;
    }
    std::println();
}

void use_for() {
    std::print("for : ");
    for (auto&& w : ListWord()) std::print("{} ", w);
    std::println();
}

int main() {
    use_iterator();
    use_for();
}

