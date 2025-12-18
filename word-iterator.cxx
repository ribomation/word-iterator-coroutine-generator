#include <print>
#include <string>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <generator>
#include <ranges>

namespace fs = std::filesystem;
namespace r = std::ranges;
namespace v = std::ranges::views;
using namespace std::string_literals;

static bool is_letter(int c) {
    if (c == std::char_traits<char>::eof()) return false;
    auto uc = static_cast<unsigned char>(static_cast<char>(c));
    return std::isalpha(uc) || uc == static_cast<unsigned char>('\'');
}

auto WordIterator(std::istream& input) -> std::generator<std::string> {
    auto word = ""s;
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

int main() {
    auto filename = fs::path{"./shakespeare.txt"};
    auto file = std::ifstream{filename};
    if (not file) throw std::invalid_argument{"cannot open "s + filename.string()};

    auto freqs = std::unordered_map<std::string, unsigned>{};
    freqs.reserve((fs::file_size(filename) / 8) / 4);

    auto nontiny = [](std::string const& w) { return w.size() >= 5; };
    auto lowcase = [](std::string w) {
        for (char& c: w) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        return w;
    };
    auto pipeline = WordIterator(file)
                    | v::filter(nontiny)
                    | v::transform(lowcase);
    for (auto&& word: pipeline) ++freqs[word];

    using WF = std::pair<std::string, unsigned>;
    auto sortable = std::vector<WF>{};
    sortable.reserve(freqs.size());
    sortable.insert(sortable.end(),
                    std::make_move_iterator(freqs.begin()),
                    std::make_move_iterator(freqs.end()));

    auto top_n = std::min(sortable.size(), 10UL);
    auto cutoff = sortable.begin() + static_cast<std::ptrdiff_t>(top_n);
    auto by_freq_desc = [](WF const& a, WF const& b) { return a.second > b.second; };
    r::partial_sort(sortable, cutoff, by_freq_desc);

    for (auto const& [word, freq]: sortable | v::take(top_n)) {
        std::println("{}: {}", word, freq);
    }
}
