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
    ./src/keep_off_the_grass/action/Action.cpp
    ./src/keep_off_the_grass/action/Build.cpp
    ./src/keep_off_the_grass/action/Message.cpp
    ./src/keep_off_the_grass/action/Move.cpp
    ./src/keep_off_the_grass/action/Spawn.cpp
    ./src/keep_off_the_grass/action/Wait.cpp
    ./src/keep_off_the_grass/bot/Bot.cpp
    ./src/keep_off_the_grass/bot/WaitBot.cpp
    ./src/keep_off_the_grass/bot/WoodBot.cpp
    ./src/keep_off_the_grass/map/Cell.cpp
    ./src/keep_off_the_grass/map/Map.cpp
    ./src/keep_off_the_grass/map/ScoredMap.cpp
    ./src/keep_off_the_grass/player/Player.cpp
    ./src/keep_off_the_grass/state/State.cpp
)

# test files *******************************************************************
set(test_files
    ./test/keep_off_the_grass/map/test_Map.cpp
    ./test/keep_off_the_grass/map/test_ScoredMap.cpp
)

# exe fall_challenge_2022 ******************************************************
add_executable(keep_off_the_grass ./src/keep_off_the_grass/main.cpp ${source_files})

# exe test_fall_challenge_2022 *************************************************
add_executable(test_keep_off_the_grass ${test_files} ${source_files})
target_link_libraries(test_keep_off_the_grass PRIVATE Catch2::Catch2WithMain)
