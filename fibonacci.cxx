#include <print>
#include <generator>
#include <utility>
namespace v = std::ranges::views;

auto fibonacci() -> std::generator<unsigned long> {
    auto f0 = 1UL, f1 = 1UL;
    while (true) {
        co_yield f0;
        f0 = std::exchange(f1, f0 + f1);
    }
}

int main(int argc, char* argv[]) {
    auto const N = argc == 1 ? 10U : std::stoi(argv[1]);
    for (auto k: fibonacci() | v::take(N)) std::print("{} ", k);
    std::println();
}
