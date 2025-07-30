#include <doctest/doctest.h>

#include <soak_overflow/grid/Direction.h>

TEST_CASE("soak_overflow::grid::Direction")
{
    SUBCASE("Given the directions contained in the class")
    {
        SUBCASE("Then east delta_x is 1 and delta_y is 0 and is orthogonal")
        {
            CHECK(soak_overflow::grid::Direction::east.delta_x == 1);
            CHECK(soak_overflow::grid::Direction::east.delta_y == 0);
            CHECK(soak_overflow::grid::Direction::east.is_orthogonal());
        }
        SUBCASE("Then north delta_x is 0 and delta_y is -1 and is orthogonal")
        {
            CHECK(soak_overflow::grid::Direction::north.delta_x == 0);
            CHECK(soak_overflow::grid::Direction::north.delta_y == -1);
            CHECK(soak_overflow::grid::Direction::north.is_orthogonal());
        }
        SUBCASE("Then north east delta_x is 1 and delta_y is -1 and is not orthogonal")
        {
            CHECK(soak_overflow::grid::Direction::north_east.delta_x == 1);
            CHECK(soak_overflow::grid::Direction::north_east.delta_y == -1);
            CHECK(!soak_overflow::grid::Direction::north_east.is_orthogonal());
        }
        SUBCASE("Then north west delta_x is -1 and delta_y is -1 and is not orthogonal")
        {
            CHECK(soak_overflow::grid::Direction::north_west.delta_x == -1);
            CHECK(soak_overflow::grid::Direction::north_west.delta_y == -1);
            CHECK(!soak_overflow::grid::Direction::north_west.is_orthogonal());
        }
        SUBCASE("Then south delta_x is 0 and delta_y is 1 and is orthogonal")
        {
            CHECK(soak_overflow::grid::Direction::south.delta_x == 0);
            CHECK(soak_overflow::grid::Direction::south.delta_y == 1);
            CHECK(soak_overflow::grid::Direction::south.is_orthogonal());
        }
        SUBCASE("Then south east delta_x is 1 and delta_y is 1 and is not orthogonal")
        {
            CHECK(soak_overflow::grid::Direction::south_east.delta_x == 1);
            CHECK(soak_overflow::grid::Direction::south_east.delta_y == 1);
            CHECK(!soak_overflow::grid::Direction::south_east.is_orthogonal());
        }
        SUBCASE("Then south west delta_x is -1 and delta_y is 1 and is not orthogonal")
        {
            CHECK(soak_overflow::grid::Direction::south_west.delta_x == -1);
            CHECK(soak_overflow::grid::Direction::south_west.delta_y == 1);
            CHECK(!soak_overflow::grid::Direction::south_west.is_orthogonal());
        }
        SUBCASE("Then west delta_x is -1 and delta_y is 0 and is orthogonal")
        {
            CHECK(soak_overflow::grid::Direction::west.delta_x == -1);
            CHECK(soak_overflow::grid::Direction::west.delta_y == 0);
            CHECK(soak_overflow::grid::Direction::west.is_orthogonal());
        }
    }

    SUBCASE("Given the orthogonal directions")
    {
        SUBCASE("Then it contains 4 elements")
        {
            CHECK(soak_overflow::grid::Direction::orthogonal_directions.size() == 4);
        }

        bool east_found = false;
        bool north_found = false;
        bool south_found = false;
        bool west_found = false;
        for (
            const soak_overflow::grid::Direction& cr_direction
            : soak_overflow::grid::Direction::orthogonal_directions
        )
        {
            if (cr_direction == soak_overflow::grid::Direction::east)   
            {
                east_found = true;
            }
            else if (cr_direction == soak_overflow::grid::Direction::north)
            {
                north_found = true;
            }
            else if (cr_direction == soak_overflow::grid::Direction::south)
            {
                south_found = true;
            }
            else if (cr_direction == soak_overflow::grid::Direction::west)
            {
                west_found = true;
            }
        }

        SUBCASE("Then it contains east, north, south and west")
        {
            CHECK(east_found);
            CHECK(north_found);
            CHECK(south_found);
            CHECK(west_found);
        }
    }

    SUBCASE("Given the directions")
    {
        SUBCASE("Then it contains 8 elements")
        {
            CHECK(soak_overflow::grid::Direction::directions.size() == 8);
        }

        bool east_found = false;
        bool north_found = false;
        bool north_east_found = false;
        bool north_west_found = false;
        bool south_found = false;
        bool south_east_found = false;
        bool south_west_found = false;
        bool west_found = false;
        for (const soak_overflow::grid::Direction& cr_direction: soak_overflow::grid::Direction::directions)
        {
            if (cr_direction == soak_overflow::grid::Direction::east)
            {
                east_found = true;
            }
            else if (cr_direction == soak_overflow::grid::Direction::north)
            {
                north_found = true;
            }
            else if (cr_direction == soak_overflow::grid::Direction::north_east)
            {
                north_east_found = true;
            }
            else if (cr_direction == soak_overflow::grid::Direction::north_west)
            {
                north_west_found = true;
            }
            else if (cr_direction == soak_overflow::grid::Direction::south)
            {
                south_found = true;
            }
            else if (cr_direction == soak_overflow::grid::Direction::south_east)
            {
                south_east_found = true;
            }
            else if (cr_direction == soak_overflow::grid::Direction::south_west)
            {
                south_west_found = true;
            }
            else if (cr_direction == soak_overflow::grid::Direction::west)
            {
                west_found = true;
            }
        }

        SUBCASE("Then it contains east, north, north east, north west, south, south east, south west and west")
        {
            CHECK(east_found);
            CHECK(north_found);
            CHECK(north_east_found);
            CHECK(north_west_found);
            CHECK(south_found);
            CHECK(south_east_found);
            CHECK(south_west_found);
            CHECK(west_found);
        }
    }
}
