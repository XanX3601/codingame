#include <doctest/doctest.h>

#include <iostream>
#include <sstream>

#include <soak_overflow/agent/AgentData.h>

TEST_CASE("soak_overflow::agent::AgentData")
{
    SUBCASE("Given an agent data")
    {
        soak_overflow::agent::AgentData agent_data(
            1, 2, 3, 4, 5, 6
        );

        SUBCASE("Then its getters give back what was set on init")
        {
            CHECK(agent_data.get_id() == 1);
            CHECK(agent_data.get_player_id() == 2);
            CHECK(agent_data.get_shoot_cooldown() == 3);
            CHECK(agent_data.get_optimal_range() == 4);
            CHECK(agent_data.get_soaking_power() == 5);
            CHECK(agent_data.get_initiali_splash_bomb_count() == 6);
        }
    }
    
    SUBCASE("Given an agent data from stdin")
    {
        std::string input("1 2 3 4 5 6");
        std::cin.rdbuf(std::istringstream(input).rdbuf());

        soak_overflow::agent::AgentData agent_data(
            soak_overflow::agent::AgentData::from_stdin()
        );

        SUBCASE("Then its getters give back what was set on stdin")
        {
            CHECK(agent_data.get_id() == 1);
            CHECK(agent_data.get_player_id() == 2);
            CHECK(agent_data.get_shoot_cooldown() == 3);
            CHECK(agent_data.get_optimal_range() == 4);
            CHECK(agent_data.get_soaking_power() == 5);
            CHECK(agent_data.get_initiali_splash_bomb_count() == 6);
        }
    }
}
