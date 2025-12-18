#include <iostream>
#include <print>
#include <string>
#include <array>
#include <generator>
#include <random>
#include <ranges>
using std::cout;
using std::string;
using namespace std::string_literals;
namespace v = std::ranges::views;

auto generate_name() -> std::generator<string> {
    static auto const names = std::array{
        "Anna"s, "Berit"s, "Carin"s, "Doris"s, "Eva"s, "Frida"s, "Gun"s,
        "Adam"s, "Betil"s, "Carl"s, "Dan"s, "Erik"s, "Fred"s, "Gunnar"s,
    };
    static auto r = std::default_random_engine{std::random_device{}()};
    static auto index = std::uniform_int_distribution<unsigned>{0, names.size() - 1};

    auto name = ""s;
    while (true) {
        name = names[index(r)];
        co_yield std::move(name);
    }
}

int main() {
    for (auto&& name : generate_name() | v::take(10)) {
        std::print("{} ", name);
    }
}
