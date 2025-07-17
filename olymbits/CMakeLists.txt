cmake_minimum_required(VERSION 3.22)
project(olymbits)

# sub directories **************************************************************
add_subdirectory(./libs)

# include directories **********************************************************
include_directories(
    ./include
)

# source files *****************************************************************
set(source_files
    ./src/olymbits/mini_game/MiniGame.cpp
)

# test files *******************************************************************
set(test_files
    ./test/truc.cpp
    ./test/olymbits/mini_game/TestMiniGame.cpp
)

# exe fall_challenge_2022 ******************************************************
add_executable(olymbits ./src/olymbits/main.cpp ${source_files})
target_compile_features(olymbits PRIVATE cxx_std_17)

# exe test_fall_challenge_2022 *************************************************
add_executable(test_olymbits ${test_files} ${source_files})
target_link_libraries(test_olymbits PRIVATE Catch2::Catch2WithMain)
target_compile_features(test_olymbits PRIVATE cxx_std_17)
