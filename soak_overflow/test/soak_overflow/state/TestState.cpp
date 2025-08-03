#include <doctest/doctest.h>

#include <iostream>
#include <sstream>

#include <soak_overflow/state/State.h>

TEST_CASE("soak_overflow::state::State")
{
    SUBCASE("Given a state")
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

        soak_overflow::state::State state(
            &agent_data_collection
        );

        input = (
            "2\n"
            "1 1 2 3 4 5\n"
            "0 2 3 4 5 6\n"
        );
        input_stream = std::istringstream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        state.update_from_stdin();

        SUBCASE("Then agent 0 is as expected")
        {
            const soak_overflow::agent::Agent& cr_agent(
                state.get_agent_collection().get_agent(0)
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
                state.get_agent_collection().get_agent(1)
            );

            CHECK(cr_agent.get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(cr_agent.get_current_shoot_cooldown() == 3);
            CHECK(cr_agent.get_current_splash_bomb_count() == 4);
            CHECK(cr_agent.get_wetness() == 5);
            CHECK(cr_agent.is_alive());
        }

        SUBCASE("When updating only agent 1")
        {
            input = (
                "1\n"
                "1 1 2 3 4 5\n"
            );
            input_stream = std::istringstream(input);
            std::cin.rdbuf(input_stream.rdbuf());

            state.update_from_stdin();

            SUBCASE("Then agent 0 is dead")
            {
                const soak_overflow::agent::Agent& cr_agent(
                    state.get_agent_collection().get_agent(0)
                );

                CHECK_FALSE(cr_agent.is_alive());
            }
        }

        SUBCASE("When copying it")
        {
            soak_overflow::state::State copy_state(state);

            SUBCASE("Then agent 0 is as expected")
            {
                const soak_overflow::agent::Agent& cr_agent(
                    copy_state.get_agent_collection().get_agent(0)
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
                    copy_state.get_agent_collection().get_agent(1)
                );

                CHECK(cr_agent.get_coord() == soak_overflow::grid::Coord(1, 2));
                CHECK(cr_agent.get_current_shoot_cooldown() == 3);
                CHECK(cr_agent.get_current_splash_bomb_count() == 4);
                CHECK(cr_agent.get_wetness() == 5);
                CHECK(cr_agent.is_alive());
            }

        }
    }
}
