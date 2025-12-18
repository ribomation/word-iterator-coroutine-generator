#include <print>
#include <generator>
#include <utility>

auto fibonacci() -> std::generator<unsigned long> {
    auto f0 = 1UL, f1 = 1UL;
    while (true) {
        co_yield f0;
        f0 = std::exchange(f1, f0 + f1);
    }
}

int main() {
    namespace v = std::ranges::views;
    for (auto k : fibonacci() | v::take(25)) {
        std::print("{} ", k);
    }
}
