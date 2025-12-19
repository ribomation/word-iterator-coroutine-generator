#include <print>
#include <string>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <generator>
#include <ranges>
#include <chrono>
#include <iterator>

#include "word-reader-coroutine.hxx"
#include "word-reader-iterator.hxx"

namespace fs = std::filesystem;
namespace c = std::chrono;
namespace r = std::ranges;
namespace v = std::ranges::views;
using namespace std::string_literals;

struct Params {
    fs::path filename = fs::path{"../data/shakespeare.txt"};
    unsigned min_word_size = 5;
    unsigned max_words = 10;
    unsigned sso_cutoff = 15;
};

void use_case(std::string const& name, Params const& params, auto readerFactory) {
    std::println("---- {} ----", name);
    auto start_time = c::steady_clock::now();
    {
        auto freqs = std::unordered_map<std::string, unsigned>{};
        freqs.reserve((fs::file_size(params.filename) / 8) / 4);

        auto non_tiny = [&params](std::string const& w) { return w.size() >= params.min_word_size; };
        auto non_sso = [&params](std::string const& w) { return w.size() <= params.sso_cutoff; };
        auto lowcase = [](std::string w) {
            for (char& c: w) {
                c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            }
            return w;
        };

        auto reader = readerFactory();
        auto pipeline = std::move(reader)
                        | v::filter(non_tiny)
                        | v::filter(non_sso)
                        | v::transform(lowcase);
        for (auto&& word: pipeline) ++freqs[word];

        using WF = std::pair<std::string, unsigned>;
        auto sortable = std::vector<WF>{};
        sortable.reserve(freqs.size());
        sortable.insert(sortable.end(),
                        std::make_move_iterator(freqs.begin()),
                        std::make_move_iterator(freqs.end()));

        auto top_n = std::min(sortable.size(), static_cast<size_t>(params.max_words));
        auto cutoff = sortable.begin() + static_cast<std::ptrdiff_t>(top_n);
        auto by_freq_desc = [](WF const& a, WF const& b) { return a.second > b.second; };
        r::partial_sort(sortable, cutoff, by_freq_desc);

        for (auto const& [word, freq]: sortable | v::take(top_n)) {
            std::println("{}: {}", word, freq);
        }
    }
    auto stop_time = c::steady_clock::now();
    auto elapsed = c::duration_cast<c::milliseconds>(stop_time - start_time);
    std::println("Elapsed time: {} ms", elapsed.count());
}


int main(int argc, char* argv[]) {
    auto params = Params{};
    for (auto k = 1; k < argc; ++k) {
        auto arg = std::string{argv[k]};
        if (arg == "--file"s) {
            params.filename = argv[++k];
        }
    }
    if (not fs::exists(params.filename)) {
        throw std::invalid_argument{"cannot open "s + params.filename.string()};
    }

    {
        auto file = std::ifstream{params.filename};
        use_case("Coroutine", params, [&file]() {
            return WordReaderCoroutine(file);
        });
    }
    {
        auto file = std::ifstream{params.filename};
        use_case("Iterator", params, [&file]() {
            auto first = WordReaderIterator{file};
            auto last = WordReaderIterator{};
            return r::subrange{first, last};
        });
    }
}
