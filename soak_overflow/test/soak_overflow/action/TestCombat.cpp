#include "soak_overflow/action/Throw.h"
#include <doctest/doctest.h>

#include <soak_overflow/action/Combat.h>

TEST_CASE("soak_overflow::action::Combat")
{
    SUBCASE("Given a shoot on target 3")
    {
        soak_overflow::action::Combat combat({3});

        SUBCASE("Then its target id is 3")
        {
            CHECK(std::get<soak_overflow::action::Shoot>(combat).get_target_id() == 3);
        }
    }

    SUBCASE("Given a throw on coord (5, 2)")
    {
        soak_overflow::action::Combat combat(soak_overflow::action::Throw({5 ,2}));

        SUBCASE("Then its coord are (5, 2)")
        {
            CHECK(
                std::get<soak_overflow::action::Throw>(combat).get_coord()
                    == soak_overflow::grid::Coord(5, 2)
            );
        }
    }
}
