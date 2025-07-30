#include <doctest/doctest.h>

#include <iostream>
#include <sstream>

#include <soak_overflow/agent/AgentDataCollection.h>

TEST_CASE("soak_overflow::agent::AgentDataCollection")
{
    SUBCASE("Given an agent data collection from stdin")
    {
        std::string input(
            "4\n"
            "0 0 1 2 3 4\n"
            "1 0 2 3 4 5\n"
            "2 1 3 4 5 6\n"
            "3 1 4 5 6 7\n"
        );
        std::istringstream input_stream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        soak_overflow::agent::AgentDataCollection agent_data_collection(
            soak_overflow::agent::AgentDataCollection::from_stdin()
        );

        SUBCASE("Then it contains 4 agent")
        {
            const std::unordered_set<int>& cr_agent_ids(
                agent_data_collection.get_agent_ids()
            );
            
            CHECK(cr_agent_ids.size() == 4);
            CHECK(cr_agent_ids.find(0) != cr_agent_ids.end());
            CHECK(cr_agent_ids.find(1) != cr_agent_ids.end());
            CHECK(cr_agent_ids.find(2) != cr_agent_ids.end());
            CHECK(cr_agent_ids.find(3) != cr_agent_ids.end());

            SUBCASE("Then agent 0 is as expected")
            {
                const soak_overflow::agent::AgentData& cr_agent_data(
                    agent_data_collection.get_agent_data(0)
                );

                CHECK(cr_agent_data.get_id() == 0);
                CHECK(cr_agent_data.get_player_id() == 0);
                CHECK(cr_agent_data.get_shoot_cooldown() == 1);
                CHECK(cr_agent_data.get_optimal_range() == 2);
                CHECK(cr_agent_data.get_soaking_power() == 3);
                CHECK(cr_agent_data.get_initiali_splash_bomb_count() == 4);
            }

            SUBCASE("Then agent 1 is as expected")
            {
                const soak_overflow::agent::AgentData& cr_agent_data(
                    agent_data_collection.get_agent_data(1)
                );

                CHECK(cr_agent_data.get_id() == 1);
                CHECK(cr_agent_data.get_player_id() == 0);
                CHECK(cr_agent_data.get_shoot_cooldown() == 2);
                CHECK(cr_agent_data.get_optimal_range() == 3);
                CHECK(cr_agent_data.get_soaking_power() == 4);
                CHECK(cr_agent_data.get_initiali_splash_bomb_count() == 5);
            }

            SUBCASE("Then agent 2 is as expected")
            {
                const soak_overflow::agent::AgentData& cr_agent_data(
                    agent_data_collection.get_agent_data(2)
                );

                CHECK(cr_agent_data.get_id() == 2);
                CHECK(cr_agent_data.get_player_id() == 1);
                CHECK(cr_agent_data.get_shoot_cooldown() == 3);
                CHECK(cr_agent_data.get_optimal_range() == 4);
                CHECK(cr_agent_data.get_soaking_power() == 5);
                CHECK(cr_agent_data.get_initiali_splash_bomb_count() == 6);
            }

            SUBCASE("Then agent 3 is as expected")
            {
                const soak_overflow::agent::AgentData& cr_agent_data(
                    agent_data_collection.get_agent_data(3)
                );

                CHECK(cr_agent_data.get_id() == 3);
                CHECK(cr_agent_data.get_player_id() == 1);
                CHECK(cr_agent_data.get_shoot_cooldown() == 4);
                CHECK(cr_agent_data.get_optimal_range() == 5);
                CHECK(cr_agent_data.get_soaking_power() == 6);
                CHECK(cr_agent_data.get_initiali_splash_bomb_count() == 7);
            }
        }

        SUBCASE("Then it contains 2 players")
        {
            const std::unordered_set<int>& cr_player_ids(
                agent_data_collection.get_player_ids()
            );

            CHECK(cr_player_ids.size() == 2);
            CHECK(cr_player_ids.find(0) != cr_player_ids.end());
            CHECK(cr_player_ids.find(1) != cr_player_ids.end());

            SUBCASE("Then player 0 agent ids are 0 and 1")
            {
                const std::unordered_set<int>& cr_agent_ids(
                    agent_data_collection.get_player_agent_ids(0)
                );

                CHECK(cr_agent_ids.size() == 2);
                CHECK(cr_agent_ids.find(0) != cr_agent_ids.end());
                CHECK(cr_agent_ids.find(1) != cr_agent_ids.end());
            }

            SUBCASE("Then player 1 agent ids are 2 and 3")
            {
                const std::unordered_set<int>& cr_agent_ids(
                    agent_data_collection.get_player_agent_ids(1)
                );

                CHECK(cr_agent_ids.size() == 2);
                CHECK(cr_agent_ids.find(2) != cr_agent_ids.end());
                CHECK(cr_agent_ids.find(3) != cr_agent_ids.end());
            }

            SUBCASE("Then player 0 enemy agent ids are 2 and 3")
            {
                const std::unordered_set<int>& cr_agent_ids(
                    agent_data_collection.get_player_enemay_agent_ids(0)
                );

                CHECK(cr_agent_ids.size() == 2);
                CHECK(cr_agent_ids.find(2) != cr_agent_ids.end());
                CHECK(cr_agent_ids.find(3) != cr_agent_ids.end());
            }

            SUBCASE("Then player 1 enemy agent ids are 0 and 1")
            {
                const std::unordered_set<int>& cr_agent_ids(
                    agent_data_collection.get_player_enemay_agent_ids(1)
                );

                CHECK(cr_agent_ids.size() == 2);
                CHECK(cr_agent_ids.find(0) != cr_agent_ids.end());
                CHECK(cr_agent_ids.find(1) != cr_agent_ids.end());
            }
        }
    }
}
