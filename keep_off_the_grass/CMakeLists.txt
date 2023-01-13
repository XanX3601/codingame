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
)

# exe fall_challenge_2022 ******************************************************
add_executable(keep_off_the_grass ./src/keep_off_the_grass/main.cpp ${source_files})

# exe test_fall_challenge_2022 *************************************************
add_executable(test_keep_off_the_grass ${test_files} ${source_files})
target_link_libraries(test_keep_off_the_grass PRIVATE Catch2::Catch2WithMain)
