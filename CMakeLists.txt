cmake_minimum_required(VERSION 3.26)
project(homework)

set(CMAKE_CXX_STANDARD 20)

set(
        CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
)

aux_source_directory(./src CPP_FILE)

add_executable(
        homework main.cpp
        ${CPP_FILE}
)

include_directories(
        ${CMAKE_SOURCE_DIR}/include
)