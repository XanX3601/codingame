#include <catch2/catch_template_test_macros.hpp>
#include <fall_challenge_2022/Map.h>
#include <sstream>

SCENARIO("Test class Map: looping through cells")
{
    GIVEN("A map of size 3x2")
    {
        fall_challenge_2022::Map map(3, 2);

        WHEN("Looping through its cell and given them a unique amount of scrap")
        {
            int scrap_amount = 1;

            for (int x(0); x < 3; ++x)
            {
                for (int y(0); y < 2; ++y)
                {
                    fall_challenge_2022::Cell& r_cell(map.get_cell(x, y));
                    r_cell.set_scrap_amount(scrap_amount);

                    scrap_amount += 1;
                }
            }

            THEN("They each have a unique amount of scrap")
            {
                int expected_scrap_amount = 1;

                for (int x(0); x < 3; ++x)
                {
                    for (int y(0); y < 2; ++y)
                    {
                        const fall_challenge_2022::Cell& cr_cell(
                            map.get_cell(x, y)
                        );
                        REQUIRE(cr_cell.get_scrap_amount() == expected_scrap_amount);

                        expected_scrap_amount += 1;
                    }
                }
            }
        }
    }
}

SCENARIO("Test class Map: instantiating map")
{
    GIVEN("A string representing a map")
    {
        std::stringstream map_stringstream;
        map_stringstream << "3x2"
            << "-"
            << "0,0,1,-1,1,0,1,0,1;"
            << "0,1,2,-1,2,0,1,0,1;"
            << "1,0,3,-1,3,0,1,0,1;"
            << "1,1,4,-1,4,0,1,0,1;"
            << "2,0,5,-1,5,0,1,0,1;"
            << "2,1,6,-1,6,0,1,0,1";

        std::string map_string(map_stringstream.str());

        WHEN("Instantiating a map from it")
        {
            fall_challenge_2022::Map map(map_string);

            THEN("The obtained map correspond to the one described by the string")
            {
                REQUIRE(map.get_width() == 3);
                REQUIRE(map.get_height() == 2);

                int expected_amount(1);

                for (int x(0); x < 3; ++x)
                {
                    for (int y(0); y < 2; ++y)
                    {
                        const fall_challenge_2022::Cell& cr_cell(
                            map.get_cell(x, y)
                        );

                        REQUIRE(cr_cell.get_scrap_amount() == expected_amount);
                        REQUIRE(cr_cell.get_owner() == fall_challenge_2022::Player::null);
                        REQUIRE(cr_cell.get_unit_amount() == expected_amount);
                        REQUIRE(!cr_cell.has_recycler());
                        REQUIRE(cr_cell.is_buildable());
                        REQUIRE(!cr_cell.is_spawnable());
                        REQUIRE(cr_cell.is_harvested());

                        ++expected_amount;
                    }
                }
            }

            THEN("The obtained map can recreated the original string")
            {
                REQUIRE(map_string == map.to_string());
            }
        }
    }
}

