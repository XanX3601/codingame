#include <catch2/catch_test_macros.hpp>

#include <olymbits/mini_game/Archery.h>
#include <olymbits/action/Action.h>

SCENARIO("Test Archery")
{
    GIVEN("An archery")
    {
        olymbits::mini_game::Archery archery;
        archery.update({0, -10, 5, 8, -2, 20}, "9914113315261");

        WHEN("Applying left, right, up")
        {
            olymbits::mini_game::Archery next_archery;

            archery.apply_to(
                olymbits::action::Action::left,
                olymbits::action::Action::right,
                olymbits::action::Action::up,
                next_archery
            );

            THEN("The following archery has the right values")
            {
                REQUIRE(next_archery.get_player_x_position(0) == -9);
                REQUIRE(next_archery.get_player_y_position(0) == -10);
                REQUIRE(next_archery.get_player_x_position(1) == 14);
                REQUIRE(next_archery.get_player_y_position(1) == 8);
                REQUIRE(next_archery.get_player_x_position(2) == -2);
                REQUIRE(next_archery.get_player_y_position(2) == 11);
            }
        }
    }
}
