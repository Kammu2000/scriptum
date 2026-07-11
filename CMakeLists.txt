cmake_minimum_required(VERSION 3.25)
project(scriptum VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic)

add_library(scriptum_lib
    src/frontend/lexer/tokenize.cpp
    src/frontend/parser/ast.cpp
    src/frontend/parser/parser_context.cpp
    src/frontend/parser/expression_parser.cpp
    src/frontend/parser/statement_parser.cpp
    src/frontend/parser/program_parser.cpp
    src/runtime/environment.cpp
    src/runtime/value.cpp
    src/runtime/return_signal.cpp
    src/runtime/evaluate_expression.cpp
    src/runtime/interpreter.cpp
    src/stdlib/globals.cpp
)

target_include_directories(scriptum_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(scriptum src/main.cpp)
target_link_libraries(scriptum PRIVATE scriptum_lib)
