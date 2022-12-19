cmake_minimum_required(VERSION 3.22)
project(fall_challenge_2022)

# sub directories **************************************************************
add_subdirectory(./libs)

# include directories **********************************************************
include_directories(
    ./include
)

# source files *****************************************************************
set(source_files
)

# test files *******************************************************************
set(test_files
    ./test/fall_challenge_2022/test_sample.cpp
)

# exe fall_challenge_2022 ******************************************************
add_executable(fall_challenge_2022 ./src/fall_challenge_2022/main.cpp ${source_files})

# exe test_fall_challenge_2022 *************************************************
add_executable(test_fall_challenge_2022 ${test_files} ${source_files})
target_link_libraries(test_fall_challenge_2022 PRIVATE Catch2::Catch2WithMain)

