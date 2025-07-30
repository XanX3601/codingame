#include <doctest/doctest.h>

#include <soak_overflow/grid/Coord.h>

TEST_CASE("soak_overflow::grid::Coord")
{
    SUBCASE("Given a new default coord")
    {
        soak_overflow::grid::Coord coord;

        SUBCASE("Then it's (0; 0)")
        {
            CHECK(coord.x == 0);
            CHECK(coord.y == 0);
        }

        SUBCASE("Then it is equal to default coord")
        {
            CHECK(coord == soak_overflow::grid::Coord());
        }
    }

    SUBCASE("Given the coord (2, 5)")
    {
        soak_overflow::grid::Coord coord(2, 5);

        SUBCASE("Then its x value is 2")
        {
            CHECK(coord.x == 2);
        }

        SUBCASE("Then its y value is 5")
        {
            CHECK(coord.y == 5);
        }

        SUBCASE("Then it is not equl to default coord")
        {
            CHECK(!(coord == soak_overflow::grid::Coord()));
        }

        SUBCASE("Then it is equalt ot coord (2, 5)")
        {
            CHECK(coord == soak_overflow::grid::Coord(2, 5));
        }

        SUBCASE("When applying directions to the coord")
        {
            soak_overflow::grid::Coord east(
                coord.apply_direction(soak_overflow::grid::Direction::east)
            );

            SUBCASE("Then east direction gives coord (3, 5)")
            {
                CHECK(east.x == 3);
                CHECK(east.y == 5);
            }

            soak_overflow::grid::Coord north(
                coord.apply_direction(soak_overflow::grid::Direction::north)
            );

            SUBCASE("Then north direction gives coord (2, 4)")
            {
                CHECK(north.x == 2);
                CHECK(north.y == 4);
            }

            soak_overflow::grid::Coord north_east(
                coord.apply_direction(soak_overflow::grid::Direction::north_east)
            );

            SUBCASE("Then north east direction gives coord (3, 4)")
            {
                CHECK(north_east.x == 3);
                CHECK(north_east.y == 4);
            }

            soak_overflow::grid::Coord north_west(
                coord.apply_direction(soak_overflow::grid::Direction::north_west)
            );

            SUBCASE("Then north west direction gives coord (1, 4)")
            {
                CHECK(north_west.x == 1);
                CHECK(north_west.y == 4);
            }

            soak_overflow::grid::Coord south(
                coord.apply_direction(soak_overflow::grid::Direction::south)
            );

            SUBCASE("Then south direction gives coord (2, 6)")
            {
                CHECK(south.x == 2);
                CHECK(south.y == 6);
            }

            soak_overflow::grid::Coord south_east(
                coord.apply_direction(soak_overflow::grid::Direction::south_east)
            );

            SUBCASE("Then south east direction gives coord (3, 6)")
            {
                CHECK(south_east.x == 3);
                CHECK(south_east.y == 6);
            }

            soak_overflow::grid::Coord south_west(
                coord.apply_direction(soak_overflow::grid::Direction::south_west)
            );

            SUBCASE("Then south west direction gives coord (1, 6)")
            {
                CHECK(south_west.x == 1);
                CHECK(south_west.y == 6);
            }

            soak_overflow::grid::Coord west(
                coord.apply_direction(soak_overflow::grid::Direction::west)
            );

            SUBCASE("Then west direction gives coord (1, 5)")
            {
                CHECK(west.x == 1);
                CHECK(west.y == 5);
            }
        }
    }
}
