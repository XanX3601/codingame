#include <doctest/doctest.h>

#include <soak_overflow/agent/Agent.h>

TEST_CASE("soak_overflow::agent::Agent")
{
    SUBCASE("Given an agent")
    {
        soak_overflow::agent::AgentData agent_data(
            1, 2, 3, 4, 5, 6
        );
        soak_overflow::agent::Agent agent(&agent_data);

        SUBCASE("When updating the agent")
        {
            agent.update(
                {2, 5},
                1,
                2,
                3
            );

            SUBCASE("Then its getters give back what is expected")
            {
                CHECK(agent.get_coord() == soak_overflow::grid::Coord(2, 5));
                CHECK(agent.get_current_shoot_cooldown() == 1);
                CHECK(agent.get_current_splash_bomb_count() == 2);
                CHECK(agent.get_wetness() == 3);
            }
        }

        SUBCASE("Then the agent is alive")
        {
            CHECK(agent.is_alive());
        }

        SUBCASE("When killing the agent")
        {
            agent.kill();

            SUBCASE("The agent is dead")
            {
                CHECK(!agent.is_alive());
            }
        }
    }
}
