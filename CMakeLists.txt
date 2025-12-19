cmake_minimum_required(VERSION 3.31)
project(word-iterator-coroutine-generator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
add_compile_options(-Wall -Wextra -O3 -march=native)

add_executable(generate-names generate-names.cxx)

add_executable(list-words list-words.cxx)
add_executable(fibonacci fibonacci.cxx)

add_executable(word-reader-comparison
    word-reader/is-letter.hxx
    word-reader/word-reader-coroutine.hxx
    word-reader/word-reader-iterator.hxx
    word-reader/word-reader-comparison.cxx
)
target_include_directories(word-reader-comparison PRIVATE word-reader)


add_executable(word-reader-demo
    word-reader/is-letter.hxx
    word-reader/word-reader-coroutine.hxx
    word-reader/word-reader-demo.cxx
)
target_include_directories(word-reader-demo PRIVATE word-reader)


