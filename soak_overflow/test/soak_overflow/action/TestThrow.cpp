#include <doctest/doctest.h>

#include <soak_overflow/action/Throw.h>
#include <soak_overflow/grid/Coord.h>

TEST_CASE("soak_overflow::action::Throw")
{
    SUBCASE("Given a throw by agent 1 on (2, 5)")
    {
        soak_overflow::action::Throw combat({2, 5});

        SUBCASE("Then its coords are (2, 5)")
        {
            CHECK(combat.get_coord() == soak_overflow::grid::Coord(2, 5));
        }
    }
}
