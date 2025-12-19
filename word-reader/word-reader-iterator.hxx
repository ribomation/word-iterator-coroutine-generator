#pragma once
#include <iterator>
#include <string>
#include <istream>
#include "is-letter.hxx"

class WordReaderIterator {
    std::istream* input{nullptr};
    std::string current_word{};
    bool at_eof{true};

public:
    using iterator_category = std::input_iterator_tag;
    using iterator_concept = std::input_iterator_tag;
    using value_type = std::string;
    using reference = const std::string&;
    using pointer = const std::string *;
    using difference_type = std::ptrdiff_t;

    WordReaderIterator() = default;
    explicit WordReaderIterator(std::istream& is) : input{&is} {
        read_next();
    }

    reference operator*() const { return current_word; }
    pointer   operator->() const { return &current_word; }

    WordReaderIterator& operator++() {
        read_next();
        return *this;
    }
    WordReaderIterator operator++(int) {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    friend bool operator==(WordReaderIterator const& a, WordReaderIterator const& b) {
        return a.input == b.input && a.at_eof == b.at_eof;
    }
    friend bool operator!=(WordReaderIterator const& a, WordReaderIterator const& b) {
        return !(a == b);
    }

private:
    void read_next() {
        if (input == nullptr) {
            at_eof = true;
            return;
        }

        current_word.clear();
        for (char ch; input->get(ch) && not is_letter(ch);) {}
        if (input->gcount()) input->unget();

        if (current_word.empty() && input->eof()) {
            at_eof = true;
            input = nullptr;
            return;
        }

        for (char ch; input->get(ch) && is_letter(ch);) {
            current_word.push_back(ch);
        }
        at_eof = false;
    }
};


