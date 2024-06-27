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
    ./src/olymbits/action/Action.cpp
    ./src/olymbits/bot/ArcheryBot.cpp
    ./src/olymbits/bot/Bot.cpp
    ./src/olymbits/bot/DivingBot.cpp
    ./src/olymbits/bot/HeuristicBot.cpp
    ./src/olymbits/bot/HurdleRaceBot.cpp
    ./src/olymbits/bot/MCTSBot.cpp
    ./src/olymbits/bot/RandomBot.cpp
    ./src/olymbits/bot/RollerSpeedSkatingBot.cpp
    ./src/olymbits/mini_game/Archery.cpp
    ./src/olymbits/mini_game/Diving.cpp
    ./src/olymbits/mini_game/HurdleRace.cpp
    ./src/olymbits/mini_game/MiniGame.cpp
    ./src/olymbits/mini_game/MiniGamePool.cpp
    ./src/olymbits/mini_game/RollerSpeedSkating.cpp
    ./src/olymbits/score_board/ScoreBoard.cpp
    ./src/olymbits/state/State.cpp
    ./src/olymbits/tools/distance.cpp
)

# test files *******************************************************************
set(test_files
    ./test/truc.cpp
)

# exe fall_challenge_2022 ******************************************************
add_executable(olymbits ./src/olymbits/main.cpp ${source_files})
target_compile_features(olymbits PRIVATE cxx_std_17)

# exe test_fall_challenge_2022 *************************************************
add_executable(test_olymbits ${test_files} ${source_files})
target_link_libraries(test_olymbits PRIVATE Catch2::Catch2WithMain)
target_compile_features(test_olymbits PRIVATE cxx_std_17)
