#pragma once
#include <generator>
#include <istream>
#include <string>
#include "is-letter.hxx"

inline auto WordReaderCoroutine(std::istream& input) -> std::generator<std::string> {
    auto word = std::string{};
    for (char ch; input.get(ch);) {
        if (is_letter(ch)) {
            word.push_back(ch);
        } else {
            if (not word.empty()) {
                co_yield std::move(word);
                word.clear();
            }
        }
    }
    if (not word.empty()) {
        co_yield std::move(word);
    }
}
