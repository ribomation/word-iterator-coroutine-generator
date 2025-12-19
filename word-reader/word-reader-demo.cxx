#include <print>
#include <sstream>
#include "word-reader-coroutine.hxx"

int main() {
    auto buffer = std::istringstream{"Anna Berit Carin Doris"};
    for (auto&& word : WordReaderCoroutine(buffer))
        std::println("{}", word);
}

