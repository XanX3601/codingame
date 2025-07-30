cmake_minimum_required(VERSION 3.22)
project(soak_overflow LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(libs/doctest)

file(GLOB_RECURSE LIB_SOURCES
     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
add_library(soak_overflow_lib ${LIB_SOURCES})
target_include_directories(
    soak_overflow_lib
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    soak_overflow
    src/soak_overflow/main.cpp
)
target_link_libraries(soak_overflow PRIVATE soak_overflow_lib)

# -----------------------------------------------------------------------------
# 4) Your test executable
# -----------------------------------------------------------------------------
file(GLOB_RECURSE TEST_SOURCES
     ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp
)
add_executable(test_soak_overflow ${TEST_SOURCES})
target_link_libraries(
    test_soak_overflow
    PRIVATE
    soak_overflow_lib
    doctest::doctest
)
enable_testing()
add_test(NAME soak_overflow_tests COMMAND test_soak_overflow)

