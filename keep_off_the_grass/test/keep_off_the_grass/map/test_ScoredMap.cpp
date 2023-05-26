#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <keep_off_the_grass/map/ScoredMap.h>

SCENARIO("Test ScoredMap: score propagation")
{
    GIVEN("A map 3x3 and a scored map")
    {
        keep_off_the_grass::map::Map map(3, 3);
        keep_off_the_grass::map::ScoredMap scored_map(map);

        WHEN("Adding 20 to cell(1, 1) with a propagation of -10")
        {
            scored_map.add_score_and_propagate(1, 1, 30, [](double score){return score - 10;});

            THEN("All cells have the proper score")
            {
                REQUIRE(scored_map.get_score(0, 0) == Catch::Approx(10));
                REQUIRE(scored_map.get_score(1, 0) == Catch::Approx(20));
                REQUIRE(scored_map.get_score(2, 0) == Catch::Approx(10));
                REQUIRE(scored_map.get_score(0, 1) == Catch::Approx(20));
                REQUIRE(scored_map.get_score(1, 1) == Catch::Approx(30));
                REQUIRE(scored_map.get_score(2, 1) == Catch::Approx(20));
                REQUIRE(scored_map.get_score(0, 2) == Catch::Approx(10));
                REQUIRE(scored_map.get_score(1, 2) == Catch::Approx(20));
                REQUIRE(scored_map.get_score(2, 2) == Catch::Approx(10));
            }

            AND_WHEN("Adding 40 to cell (0, 0) with a propagation factor of .5")
            {
                scored_map.add_score_and_propagate(0, 0, 40, [](double score){return score * .5;});

                THEN("All cells have the proper score")
                {
                    REQUIRE(scored_map.get_score(0, 0) == Catch::Approx(10 + 40));
                    REQUIRE(scored_map.get_score(1, 0) == Catch::Approx(20 + 20));
                    REQUIRE(scored_map.get_score(2, 0) == Catch::Approx(10 + 10));
                    REQUIRE(scored_map.get_score(0, 1) == Catch::Approx(20 + 20));
                    REQUIRE(scored_map.get_score(1, 1) == Catch::Approx(30 + 10));
                    REQUIRE(scored_map.get_score(2, 1) == Catch::Approx(20 + 5));
                    REQUIRE(scored_map.get_score(0, 2) == Catch::Approx(10 + 10));
                    REQUIRE(scored_map.get_score(1, 2) == Catch::Approx(20 + 5));
                    REQUIRE(scored_map.get_score(2, 2) == Catch::Approx(10 + 2.5));
                }
            }
        }
    }
}

SCENARIO("Test ScoredMap: scored map operation")
{
    GIVEN("A map 3x3 and two scored map on which add default scores")
    {
        keep_off_the_grass::map::Map map(3, 3);
        keep_off_the_grass::map::ScoredMap scored_map_1(map);
        keep_off_the_grass::map::ScoredMap scored_map_2(map);

        scored_map_1.add_score(0, 0, 10);
        scored_map_1.add_score(1, 0, 20);
        scored_map_1.add_score(2, 0, 30);
        scored_map_1.add_score(0, 1, 40);
        scored_map_1.add_score(1, 1, 50);
        scored_map_1.add_score(2, 1, 60);
        scored_map_1.add_score(0, 2, 70);
        scored_map_1.add_score(1, 2, 80);
        scored_map_1.add_score(2, 2, 90);

        scored_map_2.add_score(0, 0, 90);
        scored_map_2.add_score(1, 0, 80);
        scored_map_2.add_score(2, 0, 70);
        scored_map_2.add_score(0, 1, 60);
        scored_map_2.add_score(1, 1, 50);
        scored_map_2.add_score(2, 1, 40);
        scored_map_2.add_score(0, 2, 30);
        scored_map_2.add_score(1, 2, 20);
        scored_map_2.add_score(2, 2, 10);

        WHEN("Adding scored map 2 to scored map 1 with a factor of .5")
        {
            keep_off_the_grass::map::ScoredMap scored_map(
                scored_map_1.add_scored_map(scored_map_2, .5)
            );

            THEN("The resulting scored map have the proper scores")
            {
                REQUIRE(scored_map.get_score(0, 0) == Catch::Approx(10 + 90 / 2));
                REQUIRE(scored_map.get_score(1, 0) == Catch::Approx(20 + 80 / 2));
                REQUIRE(scored_map.get_score(2, 0) == Catch::Approx(30 + 70 / 2));
                REQUIRE(scored_map.get_score(0, 1) == Catch::Approx(40 + 60 / 2));
                REQUIRE(scored_map.get_score(1, 1) == Catch::Approx(50 + 50 / 2));
                REQUIRE(scored_map.get_score(2, 1) == Catch::Approx(60 + 40 / 2));
                REQUIRE(scored_map.get_score(0, 2) == Catch::Approx(70 + 30 / 2));
                REQUIRE(scored_map.get_score(1, 2) == Catch::Approx(80 + 20 / 2));
                REQUIRE(scored_map.get_score(2, 2) == Catch::Approx(90 + 10 / 2));
            }
        }

        WHEN("Adding scored map 2 to scored map 1 in place")
        {
            scored_map_1.add_scored_map_in_place(scored_map_2);

            THEN("Scored map 1 have the proper scores")
            {
                REQUIRE(scored_map_1.get_score(0, 0) == Catch::Approx(10 + 90));
                REQUIRE(scored_map_1.get_score(1, 0) == Catch::Approx(20 + 80));
                REQUIRE(scored_map_1.get_score(2, 0) == Catch::Approx(30 + 70));
                REQUIRE(scored_map_1.get_score(0, 1) == Catch::Approx(40 + 60));
                REQUIRE(scored_map_1.get_score(1, 1) == Catch::Approx(50 + 50));
                REQUIRE(scored_map_1.get_score(2, 1) == Catch::Approx(60 + 40));
                REQUIRE(scored_map_1.get_score(0, 2) == Catch::Approx(70 + 30));
                REQUIRE(scored_map_1.get_score(1, 2) == Catch::Approx(80 + 20));
                REQUIRE(scored_map_1.get_score(2, 2) == Catch::Approx(90 + 10));
            }
        }
    }
}


SCENARIO("Test ScoredMap: max neighbor")
{
    GIVEN("A 3x3 map and a scored map with different score for each cell")
    {
        keep_off_the_grass::map::Map map(3, 3);
        keep_off_the_grass::map::ScoredMap scored_map(map);

        scored_map.add_score(0, 0, 1);
        scored_map.add_score(1, 0, 2);
        scored_map.add_score(2, 0, 3);
        scored_map.add_score(0, 1, 4);
        scored_map.add_score(1, 1, 5);
        scored_map.add_score(2, 1, 6);
        scored_map.add_score(0, 2, 7);
        scored_map.add_score(1, 2, 8);
        scored_map.add_score(2, 2, 9);

        THEN("The max neighbor of the cell (0, 0) is the cell (0, 1)")
        {
            const auto& cr_neighbor(scored_map.get_max_neighbor(0, 0));

            REQUIRE(cr_neighbor.get_x() == 0);
            REQUIRE(cr_neighbor.get_y() == 1);
        }
        THEN("The max neighbor of the cell (1, 0) is the cell (1, 1)")
        {
            const auto& cr_neighbor(scored_map.get_max_neighbor(1, 0));

            REQUIRE(cr_neighbor.get_x() == 1);
            REQUIRE(cr_neighbor.get_y() == 1);
        }
        THEN("The max neighbor of the cell (2, 0) is the cell (2, 1)")
        {
            const auto& cr_neighbor(scored_map.get_max_neighbor(2, 0));

            REQUIRE(cr_neighbor.get_x() == 2);
            REQUIRE(cr_neighbor.get_y() == 1);
        }
        THEN("The max neighbor of the cell (0, 1) is the cell (0, 2)")
        {
            const auto& cr_neighbor(scored_map.get_max_neighbor(0, 1));

            REQUIRE(cr_neighbor.get_x() == 0);
            REQUIRE(cr_neighbor.get_y() == 2);
        }
        THEN("The max neighbor of the cell (1, 1) is the cell (1, 2)")
        {
            const auto& cr_neighbor(scored_map.get_max_neighbor(1, 1));

            REQUIRE(cr_neighbor.get_x() == 1);
            REQUIRE(cr_neighbor.get_y() == 2);
        }
        THEN("The max neighbor of the cell (2, 1) is the cell (2, 2)")
        {
            const auto& cr_neighbor(scored_map.get_max_neighbor(2, 1));

            REQUIRE(cr_neighbor.get_x() == 2);
            REQUIRE(cr_neighbor.get_y() == 2);
        }
        THEN("The max neighbor of the cell (0, 2) is the cell (1, 2)")
        {
            const auto& cr_neighbor(scored_map.get_max_neighbor(0, 2));

            REQUIRE(cr_neighbor.get_x() == 1);
            REQUIRE(cr_neighbor.get_y() == 2);
        }
        THEN("The max neighbor of the cell (1, 2) is the cell (2, 2)")
        {
            const auto& cr_neighbor(scored_map.get_max_neighbor(1, 2));

            REQUIRE(cr_neighbor.get_x() == 2);
            REQUIRE(cr_neighbor.get_y() == 2);
        }
        THEN("The max neighbor of the cell (2, 2) is the cell (1, 2)")
        {
            const auto& cr_neighbor(scored_map.get_max_neighbor(2, 2));

            REQUIRE(cr_neighbor.get_x() == 1);
            REQUIRE(cr_neighbor.get_y() == 2);
        }
    }
}
