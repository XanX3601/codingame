#include <doctest/doctest.h>

#include <iostream>
#include <sstream>

#include <soak_overflow/agent/AgentCollection.h>

TEST_CASE("soak_overflow::agent::AgentCollection")
{
    SUBCASE("Given a agent collection")
    {
        std::string input(
            "2\n"
            "0 0 1 2 3 4\n"
            "1 1 2 3 4 5\n"
        );
        std::istringstream input_stream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        soak_overflow::agent::AgentDataCollection agent_data_collection(
            soak_overflow::agent::AgentDataCollection::from_stdin()
        );

        soak_overflow::agent::AgentCollection agent_collection(
            &agent_data_collection
        );

        input = (
            "2\n"
            "1 1 2 3 4 5\n"
            "0 2 3 4 5 6\n"
        );
        input_stream = std::istringstream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        agent_collection.update_from_stdin();

        SUBCASE("Then agent 0 is as expected")
        {
            const soak_overflow::agent::Agent& cr_agent(
                agent_collection.get_agent(0)
            );

            CHECK(cr_agent.get_coord() == soak_overflow::grid::Coord(2, 3));
            CHECK(cr_agent.get_current_shoot_cooldown() == 4);
            CHECK(cr_agent.get_current_splash_bomb_count() == 5);
            CHECK(cr_agent.get_wetness() == 6);
            CHECK(cr_agent.is_alive());
        }

        SUBCASE("Then agent 1 is as expected")
        {
            const soak_overflow::agent::Agent& cr_agent(
                agent_collection.get_agent(1)
            );

            CHECK(cr_agent.get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(cr_agent.get_current_shoot_cooldown() == 3);
            CHECK(cr_agent.get_current_splash_bomb_count() == 4);
            CHECK(cr_agent.get_wetness() == 5);
            CHECK(cr_agent.is_alive());
        }

        SUBCASE("When considering coords (0, 1), (1, 2), (2, 3), and (1, 0)")
        {
            SUBCASE("Then there is no agent on (0, 1)")
            {
                CHECK_FALSE(agent_collection.is_there_an_agent_here({0, 1}));
            }
            SUBCASE("Then there is an agent on (1, 2)")
            {
                CHECK(agent_collection.is_there_an_agent_here({1, 2}));
                CHECK(agent_collection.get_agent_id_on({1, 2}) == 1);
                CHECK(
                    agent_collection.get_agent_on({1, 2}).get_coord()
                        == soak_overflow::grid::Coord(1, 2)
                );
            }
            SUBCASE("Then there is an agent on (2, 3)")
            {
                CHECK(agent_collection.is_there_an_agent_here({2, 3}));
                CHECK(agent_collection.get_agent_id_on({2, 3}) == 0);
                CHECK(
                    agent_collection.get_agent_on({2, 3}).get_coord()
                        == soak_overflow::grid::Coord(2, 3)
                );
            }
            SUBCASE("Then there is no agent on (1, 0)")
            {
                CHECK_FALSE(agent_collection.is_there_an_agent_here({1, 0}));
            }
        }

        SUBCASE("When updating only agent 1")
        {
            input = (
                "1\n"
                "1 2 3 3 4 5\n"
            );
            input_stream = std::istringstream(input);
            std::cin.rdbuf(input_stream.rdbuf());

            agent_collection.update_from_stdin();

            SUBCASE("Then agent 0 is dead")
            {
                const soak_overflow::agent::Agent& cr_agent(
                    agent_collection.get_agent(0)
                );

                CHECK_FALSE(cr_agent.is_alive());
            }

            SUBCASE("When considering coords (0, 1), (1, 2), (2, 3), and (1, 0)")
            {
                SUBCASE("Then there is no agent on (0, 1)")
                {
                    CHECK_FALSE(agent_collection.is_there_an_agent_here({0, 1}));
                }
                SUBCASE("Then there is no agent on (1, 2)")
                {
                    CHECK_FALSE(agent_collection.is_there_an_agent_here({1, 2}));
                }
                SUBCASE("Then there is an agent on (2, 3)")
                {
                    CHECK(agent_collection.is_there_an_agent_here({2, 3}));
                    CHECK(agent_collection.get_agent_id_on({2, 3}) == 1);
                    CHECK(
                        agent_collection.get_agent_on({2, 3}).get_coord()
                            == soak_overflow::grid::Coord(2, 3)
                    );
                }
                SUBCASE("Then there is no agent on (1, 0)")
                {
                    CHECK_FALSE(agent_collection.is_there_an_agent_here({1, 0}));
                }
            }
        }

        SUBCASE("When crouching agent 0")
        {
            CHECK_FALSE(agent_collection.get_agent(0).is_crouched());

            agent_collection.crouch_agent(0);

            SUBCASE("Then agent 0 is crouched")
            {
                CHECK(agent_collection.get_agent(0).is_crouched());
            }

            SUBCASE("When raising agent 0")
            {
                agent_collection.raise_agent(0);

                SUBCASE("Then agent 0 is not crouched")
                {
                    CHECK_FALSE(agent_collection.get_agent(0).is_crouched());
                }
            }
        }

        SUBCASE("When copying it")
        {
            soak_overflow::agent::AgentCollection copy_agent_collection(
                agent_collection
            );

            SUBCASE("Then the agent 0 is as expected")
            {
                const soak_overflow::agent::Agent& cr_agent(
                    copy_agent_collection.get_agent(0)
                );

                CHECK(cr_agent.get_coord() == soak_overflow::grid::Coord(2, 3));
                CHECK(cr_agent.get_current_shoot_cooldown() == 4);
                CHECK(cr_agent.get_current_splash_bomb_count() == 5);
                CHECK(cr_agent.get_wetness() == 6);
                CHECK(cr_agent.is_alive());
            }

            SUBCASE("Then agent 1 is as expected")
            {
                const soak_overflow::agent::Agent& cr_agent(
                    copy_agent_collection.get_agent(1)
                );

                CHECK(cr_agent.get_coord() == soak_overflow::grid::Coord(1, 2));
                CHECK(cr_agent.get_current_shoot_cooldown() == 3);
                CHECK(cr_agent.get_current_splash_bomb_count() == 4);
                CHECK(cr_agent.get_wetness() == 5);
                CHECK(cr_agent.is_alive());
            }

            SUBCASE("Then they share the same agent data collection")
            {
                CHECK(
                    agent_collection.get_agent_data_collection()
                        == copy_agent_collection.get_agent_data_collection()
                );
            }
        }
    }
}
