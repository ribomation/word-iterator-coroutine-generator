#pragma once
#include <string>
#include <cctype>

inline bool is_letter(int c) {
    if (c == std::char_traits<char>::eof()) return false;
    auto uc = static_cast<unsigned char>(static_cast<char>(c));
    return std::isalpha(uc) || uc == static_cast<unsigned char>('\'');
}
