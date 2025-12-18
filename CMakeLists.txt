cmake_minimum_required(VERSION 3.31)
project(word_count_stepwise_optimization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
add_compile_options(-Wall -Wextra -O3 -march=native)

add_executable(generate-names generate-names.cxx)

add_executable(list-words list-words.cxx)
add_executable(fibonacci fibonacci.cxx)
add_executable(word-iterator word-iterator.cxx)
