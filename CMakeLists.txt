cmake_minimum_required(VERSION 3.10)
project(IntelliEditor)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(llama.cpp/include)

add_subdirectory(llama.cpp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
