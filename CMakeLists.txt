cmake_minimum_required(VERSION 3.10)
project(IntelliEditor)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(llama.cpp/include)

add_subdirectory(llama.cpp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Programme principal (DEV-A)
add_executable(IntelliEditor src/main.c)
target_link_libraries(IntelliEditor llama)

# Test DEV-C : module LLM
add_executable(test_llm
    src/llm/test_llm.c
    src/llm/llm_interface.c
)
target_link_libraries(test_llm llama)

# Test DEV-C : Hunspell
add_executable(test_hunspell
    src/nlp/test_hunspell.c
    src/nlp/hunspell_wrap.c
)
target_link_libraries(test_hunspell hunspell-1.7)
