#include <doctest/doctest.h>

#include <soak_overflow/action/Combat.h>

TEST_CASE("soak_overflow::action::Combat")
{
    SUBCASE("Given a shoot on coord (2, 5)")
    {
        soak_overflow::action::Combat combat({2, 5}, false);

        SUBCASE("Then its coord are (2, 5)")
        {
            CHECK(combat.get_coord() == soak_overflow::grid::Coord(2, 5));
        }
        SUBCASE("Then it is not thrown")
        {
            CHECK_FALSE(combat.is_thrown());
        }
    }

    SUBCASE("Given a throw on coord (5, 2)")
    {
        soak_overflow::action::Combat combat({5, 2}, true);

        SUBCASE("Then its coord are (5, 2)")
        {
            CHECK(combat.get_coord() == soak_overflow::grid::Coord(5, 2));
        }
        SUBCASE("Then it is thrown")
        {
            CHECK(combat.is_thrown());
        }
    }
}
