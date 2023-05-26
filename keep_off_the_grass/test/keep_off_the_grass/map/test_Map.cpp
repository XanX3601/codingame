#include <catch2/catch_test_macros.hpp>
#include <keep_off_the_grass/map/Map.h>

SCENARIO("Test Map: graph is well setup")
{
    GIVEN("A map")
    {
        keep_off_the_grass::map::Map map(3, 3);

        THEN("All cells have the proper number of neighbors")
        {
            REQUIRE(map.get_cell(0, 0).get_neighbors().size() == 2);
            REQUIRE(map.get_cell(1, 0).get_neighbors().size() == 3);
            REQUIRE(map.get_cell(2, 0).get_neighbors().size() == 2);
            REQUIRE(map.get_cell(0, 1).get_neighbors().size() == 3);
            REQUIRE(map.get_cell(1, 1).get_neighbors().size() == 4);
            REQUIRE(map.get_cell(2, 1).get_neighbors().size() == 3);
            REQUIRE(map.get_cell(0, 2).get_neighbors().size() == 2);
            REQUIRE(map.get_cell(1, 2).get_neighbors().size() == 3);
            REQUIRE(map.get_cell(2, 2).get_neighbors().size() == 2);
        }

        WHEN("Setting the scrap amount on cell (0, 1) to 3")
        {
            map.get_cell(0, 1).set_scrap_count(3);

            THEN("The south neighbor of cell (0, 0) has a scrap amount of 3")
            {
                REQUIRE(
                    map.get_cell(0, 0).get_neighbor(keep_off_the_grass::map::Direction::south).get_scrap_count() == 3
                );
            }
        }
    }
}

