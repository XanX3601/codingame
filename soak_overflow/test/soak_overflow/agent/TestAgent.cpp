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
                CHECK(agent.get_agent_data() == &agent_data);
            }

            SUBCASE("When copying the agent")
            {
                soak_overflow::agent::Agent copy_agent(agent);

                SUBCASE("Then the getters of the copy give back what is expected")
                {
                    CHECK(copy_agent.get_coord() == soak_overflow::grid::Coord(2, 5));
                    CHECK(copy_agent.get_current_shoot_cooldown() == 1);
                    CHECK(copy_agent.get_current_splash_bomb_count() == 2);
                    CHECK(copy_agent.get_wetness() == 3);
                    CHECK(copy_agent.get_agent_data() == &agent_data);
                }
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
                CHECK_FALSE(agent.is_alive());
            }
        }

        SUBCASE("Then the agent is not crouched")
        {
            CHECK_FALSE(agent.is_crouched());
        }
        
        SUBCASE("When the agent hunker down")
        {
            agent.crouch();

            SUBCASE("Then the agent is crouched")
            {
                CHECK(agent.is_crouched());

                SUBCASE("When the agent stand up")
                {
                    agent.stand_up();

                    SUBCASE("Then the agent is not crouched anymore")
                    {
                        CHECK_FALSE(agent.is_crouched());
                    }
                }
            }
        }

        SUBCASE("When moving the agent")
        {
            CHECK(agent.get_coord() == soak_overflow::grid::Coord(0, 0));

            agent.set_coord(soak_overflow::grid::Coord(1, 2));

            SUBCASE("Then the agent moves")
            {
                CHECK(agent.get_coord() == soak_overflow::grid::Coord(1, 2));
            }
        }

        SUBCASE("Then it has stored the given agent data")
        {
            CHECK(agent.get_agent_data() == &agent_data);
        }
    }
}
