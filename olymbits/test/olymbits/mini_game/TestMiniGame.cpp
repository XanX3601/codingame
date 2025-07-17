#include <catch2/catch_test_macros.hpp>

#include <olymbits/mini_game/MiniGame.h>

SCENARIO("Considering mini game")
{
    GIVEN("A new mini game")
    {
        olymbits::mini_game::MiniGame mini_game;

        THEN("It should have 7 registers")
        {
            REQUIRE(mini_game.register_count == 7);
        }

        THEN("Its GPU display game over")
        {
            REQUIRE(mini_game.get_gpu() == "GAME OVER");
        }

        THEN("All its register should equal 0")
        {
            for (
                std::size_t register_index(0);
                register_index < mini_game.register_count;
                ++register_index
            )
            {
                REQUIRE(mini_game.get_register(register_index) == 0);
            }
        }

    }
}
