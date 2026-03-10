#include <cinttypes>
#include <doctest/doctest.h>

#include <iostream>
#include <sstream>

#include <soak_overflow/agent/AgentDataCollection.h>
#include <soak_overflow/grid/Grid.h>
#include <soak_overflow/state/Simulator.h>
#include <soak_overflow/state/State.h>

TEST_CASE("soak_overflow::state::Simulator")
{
    GIVEN("A 2x2 grid with a single agent")
    {
        soak_overflow::grid::Grid grid(2, 2);

        std::string input(
            "1\n"
            "0 0 0 0 0 0\n"
        );
        std::istringstream input_stream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        soak_overflow::agent::AgentDataCollection agent_data_collection(
            soak_overflow::agent::AgentDataCollection::from_stdin()
        );

        soak_overflow::state::State state(&agent_data_collection);

        input = (
            "1\n"
            "0 0 0 0 0 0\n"
        );
        input_stream = std::istringstream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        state.update_from_stdin();

        SUBCASE("Then the agent can move freely inside the grid")
        {
            soak_overflow::state::Simulator simulator(grid);

            // cannot go left cause outside grid
            soak_overflow::state::State next_state(
                simulator.apply_to(state, {{0, &soak_overflow::action::Move::left}})
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(0, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            // cannot go up cause outside grid
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::up}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(0, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            // can go right
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::right}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            // can not go up cause outside grid
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::up}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            // can not go right cause outside grid
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::right}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            // can go down
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::down}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            // can not go right cause outside grid
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::right}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            // can not go down cause outside grid
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::down}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            // can go left
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::left}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            // can not go down cause outside grid
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::down}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            // can not go left cause outside grid
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::left}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            // can go up
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::up}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(0, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            // crouch if stay
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::stay}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(0, 0));
            CHECK(next_state.get_agent_collection().get_agent(0).is_crouched());
            // go the other way around without testing collision againt edges
            // as it has already been checked
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::down}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::right}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::up}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::left}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(0, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
        }
    }

    GIVEN("A 3x3 horizontal grid with a single agent")
    {
        soak_overflow::grid::Grid grid(3, 3);

        std::string input(
            "0 0 1 1 0 2 2 0 1\n"
            "0 1 0 1 1 0 2 1 0\n"
            "0 2 2 1 2 1 2 2 2\n"
        );
        std::istringstream input_stream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        grid.update_from_stdin();

        input = (
            "1\n"
            "0 0 0 0 0 0\n"
        );
        input_stream = std::istringstream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        soak_overflow::agent::AgentDataCollection agent_data_collection(
            soak_overflow::agent::AgentDataCollection::from_stdin()
        );

        soak_overflow::state::State state(&agent_data_collection);

        input = (
            "1\n"
            "0 0 1 0 0 0\n"
        );
        input_stream = std::istringstream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        state.update_from_stdin();

        SUBCASE("Then the agent can go right and left on the empty line")
        {
            soak_overflow::state::Simulator simulator(grid);

            // can not go left as it is outside the grid
            soak_overflow::state::State next_state(
                simulator.apply_to(state, {{0, &soak_overflow::action::Move::left}})
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(0, 1));
            // can not go up as there is a crate
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::up}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(0, 1));
            // can not go down as there is a crate
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::down}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(0, 1));
            // can go right
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::right}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            // can not go up as there is a crate
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::up}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            // can not go down as there is a crate
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::down}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            // can go right
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::right}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(2, 1));
            // can not go up as there is a crate
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::up}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(2, 1));
            // can not go down as there is a crate
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::down}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(2, 1));
            // can not go right as it is outside the grid
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::right}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(2, 1));
            // can go left
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::left}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            // can go left
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::left}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(0, 1));
        }
    }

    GIVEN("A 3x3 vertical grid with a single agent")
    {
        soak_overflow::grid::Grid grid(3, 3);

        std::string input(
            "0 0 1 1 0 0 2 0 2\n"
            "0 1 2 1 1 0 2 1 1\n"
            "0 2 1 1 2 0 2 2 2\n"
        );
        std::istringstream input_stream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        grid.update_from_stdin();

        input = (
            "1\n"
            "0 0 0 0 0 0\n"
        );
        input_stream = std::istringstream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        soak_overflow::agent::AgentDataCollection agent_data_collection(
            soak_overflow::agent::AgentDataCollection::from_stdin()
        );

        soak_overflow::state::State state(&agent_data_collection);

        input = (
            "1\n"
            "0 1 2 0 0 0\n"
        );
        input_stream = std::istringstream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        state.update_from_stdin();

        SUBCASE("Then the agent can go up and down on the empty column")
        {
            soak_overflow::state::Simulator simulator(grid);

            // can not go down as it is outside the grid
            soak_overflow::state::State next_state(
                simulator.apply_to(state, {{0, &soak_overflow::action::Move::down}})
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 2));
            // can not go left as there is a crate
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::left}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 2));
            // can not go right as there is a crate
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::right}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 2));
            // can go up
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::up}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            // can not go left as there is a crate
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::left}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            // can not go right as there is a crate
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::right}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            // can go up
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::up}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 0));
            // can not go left as there is a crate
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::left}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 0));
            // can not go right as there is a crate
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::right}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 0));
            // can not go up as it is outside the grid
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::up}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 0));
            // can go down
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::down}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            // can go down
            next_state = simulator.apply_to(
                next_state, {{0, &soak_overflow::action::Move::down}}
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 2));
        }
    }

    GIVEN("A 3x3 vertical grid with 5 agents")
    {
        soak_overflow::grid::Grid grid(3, 3);

        std::string input(
            "0 0 0 1 0 0 2 0 0\n"
            "0 1 0 1 1 0 2 1 0\n"
            "0 2 2 1 2 0 2 2 1\n"
        );
        std::istringstream input_stream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        grid.update_from_stdin();

        input = (
            "5\n"
            "0 0 0 0 0 0\n"
            "1 1 1 1 1 1\n"
            "2 2 2 2 2 2\n"
            "3 3 3 3 3 3\n"
            "4 4 4 4 4 4\n"
        );
        input_stream = std::istringstream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        soak_overflow::agent::AgentDataCollection agent_data_collection(
            soak_overflow::agent::AgentDataCollection::from_stdin()
        );

        soak_overflow::state::State state(&agent_data_collection);

        input = (
            "5\n"
            "0 1 1 0 0 0\n"
            "1 1 0 1 1 1\n"
            "2 2 1 2 2 2\n"
            "3 1 2 3 3 3\n"
            "4 0 1 4 4 4\n"
        );
        input_stream = std::istringstream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        state.update_from_stdin();

        SUBCASE("Then agents can move")
        {
            soak_overflow::state::Simulator simulator(grid);

            // if all agent stay they are all crouched
            soak_overflow::state::State next_state(
                simulator.apply_to(
                    state,
                    {
                        {0, &soak_overflow::action::Move::stay},
                        {1, &soak_overflow::action::Move::stay},
                        {2, &soak_overflow::action::Move::stay},
                        {3, &soak_overflow::action::Move::stay},
                        {4, &soak_overflow::action::Move::stay},
                    }
                )
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            // and if they all stay but one, only the moving one is not crouched
            // anymore
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::up},
                    {1, &soak_overflow::action::Move::stay},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            // same if another move and 0 stays
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::stay},
                    {1, &soak_overflow::action::Move::stay},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::right},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            // again same but with one that actually moves
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::stay},
                    {1, &soak_overflow::action::Move::left},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(0, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            // go back to default state
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::stay},
                    {1, &soak_overflow::action::Move::right},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(1).is_crouched());
            // now if 0 tries to move, it cannot
            for (const soak_overflow::action::Move& cr_move: soak_overflow::action::Move::moves)
            {
                if (cr_move == soak_overflow::action::Move::stay)
                {
                    continue;
                }

                next_state = simulator.apply_to(
                    next_state,
                    {
                        {0, &cr_move},
                        {1, &soak_overflow::action::Move::stay},
                        {2, &soak_overflow::action::Move::stay},
                        {3, &soak_overflow::action::Move::stay},
                        {4, &soak_overflow::action::Move::stay},
                    }
                );
                CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
                CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
                CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
                CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
                CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
                CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
                CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
                CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
                CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
                CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            }
            // if 0 moves while 1, 2 and 4 move, it moves on their coord
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::up},
                    {1, &soak_overflow::action::Move::left},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(0, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::right},
                    {1, &soak_overflow::action::Move::right},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(2, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::left},
                    {1, &soak_overflow::action::Move::left},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(0, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::down},
                    {1, &soak_overflow::action::Move::right},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::right},
                    {1, &soak_overflow::action::Move::stay},
                    {2, &soak_overflow::action::Move::up},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::left},
                    {1, &soak_overflow::action::Move::stay},
                    {2, &soak_overflow::action::Move::down},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::right},
                    {1, &soak_overflow::action::Move::stay},
                    {2, &soak_overflow::action::Move::down},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::left},
                    {1, &soak_overflow::action::Move::stay},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::up},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::right},
                    {1, &soak_overflow::action::Move::stay},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::down},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::left},
                    {1, &soak_overflow::action::Move::stay},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::down},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(4).is_crouched());
            // two agent going for the same cell stay where they are
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::stay},
                    {1, &soak_overflow::action::Move::left},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::up},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::stay},
                    {1, &soak_overflow::action::Move::right},
                    {2, &soak_overflow::action::Move::up},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            // two agent trying to exchange their place stay where they are
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::down},
                    {1, &soak_overflow::action::Move::stay},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::up},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::up},
                    {1, &soak_overflow::action::Move::left},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(0, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::down},
                    {1, &soak_overflow::action::Move::stay},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(0, 0));
            CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::up},
                    {1, &soak_overflow::action::Move::stay},
                    {2, &soak_overflow::action::Move::stay},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(0, 0));
            CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::down},
                    {1, &soak_overflow::action::Move::stay},
                    {2, &soak_overflow::action::Move::left},
                    {3, &soak_overflow::action::Move::stay},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(0, 0));
            CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::down},
                    {1, &soak_overflow::action::Move::stay},
                    {2, &soak_overflow::action::Move::left},
                    {3, &soak_overflow::action::Move::up},
                    {4, &soak_overflow::action::Move::stay},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(0, 0));
            CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK(next_state.get_agent_collection().get_agent(4).is_crouched());
            next_state = simulator.apply_to(
                next_state,
                {
                    {0, &soak_overflow::action::Move::down},
                    {1, &soak_overflow::action::Move::stay},
                    {2, &soak_overflow::action::Move::left},
                    {3, &soak_overflow::action::Move::up},
                    {4, &soak_overflow::action::Move::right},
                }
            );
            CHECK(next_state.get_agent_collection().get_agent(0).get_coord() == soak_overflow::grid::Coord(1, 0));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(0).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(1).get_coord() == soak_overflow::grid::Coord(0, 0));
            CHECK(next_state.get_agent_collection().get_agent(1).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(2).get_coord() == soak_overflow::grid::Coord(2, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(2).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(3).get_coord() == soak_overflow::grid::Coord(1, 2));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(3).is_crouched());
            CHECK(next_state.get_agent_collection().get_agent(4).get_coord() == soak_overflow::grid::Coord(0, 1));
            CHECK_FALSE(next_state.get_agent_collection().get_agent(4).is_crouched());
        }
    }

    SUBCASE("An agent on a target practice")
    {
        soak_overflow::grid::Grid grid(50, 1);

        std::string input(
            "4\n"
            "0 0 0 10 10 0\n"
            "1 0 0 10 10 0\n"
            "2 0 0 10 10 0\n"
            "3 0 0 10 10 0\n"
        );
        std::istringstream input_stream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        soak_overflow::agent::AgentDataCollection agent_data_collection(
            soak_overflow::agent::AgentDataCollection::from_stdin()
        );

        soak_overflow::state::State state(&agent_data_collection);

        input = (
            "4\n"
            "0 0 0 0 0 0\n"
            "1 10 0 0 0 0\n"
            "2 20 0 0 0 0\n"
            "3 40 0 0 0 0\n"
        );
        input_stream = std::istringstream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        state.update_from_stdin();

        SUBCASE("When agent 0 shoot agent 1")
        {
            soak_overflow::state::Simulator simulator(grid);
            simulator.apply_to(
                state,
                {
                    {0, soak_overflow::action::Shoot(1)}
                }
            );

            SUBCASE("Then agent 1 has 10 wetness")
            {
                CHECK(state.get_agent_collection().get_agent(1).get_wetness() == 10);
            }
        }

        SUBCASE("When agent 0 shoot agent 2")
        {
            soak_overflow::state::Simulator simulator(grid);
            simulator.apply_to(
                state,
                {
                    {0, soak_overflow::action::Shoot(2)}
                }
            );

            SUBCASE("Then agent 2 has 5 wetness")
            {
                CHECK(state.get_agent_collection().get_agent(2).get_wetness() == 5);
            }
        }

        SUBCASE("When agent 0 shoot agent 3")
        {
            soak_overflow::state::Simulator simulator(grid);
            simulator.apply_to(
                state,
                {
                    {0, soak_overflow::action::Shoot(3)}
                }
            );

            SUBCASE("Then agent 3 has 0 wetness")
            {
                CHECK(state.get_agent_collection().get_agent(3).get_wetness() == 0);
            }
        }
    }

    SUBCASE("An agent on a target practice surrounded by crates")
    {
        soak_overflow::grid::Grid grid(50, 3);

        std::stringstream string_stream;
        for (int y(0); y < 3; ++y)
        {
            for (int x(0); x < 50; ++x)
            {
                string_stream
                    << x
                    << " "
                    << y
                    << (y % 2 == 1 ? 0 : 1)
                    << " ";
            }
            string_stream << "\n";
        }
        std::string input(string_stream.str());
        std::istringstream input_stream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        input = (
            "4\n"
            "0 0 0 10 10 0\n"
            "1 0 0 10 10 0\n"
            "2 0 0 10 10 0\n"
            "3 0 0 10 10 0\n"
        );
        input_stream = std::istringstream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        soak_overflow::agent::AgentDataCollection agent_data_collection(
            soak_overflow::agent::AgentDataCollection::from_stdin()
        );

        soak_overflow::state::State state(&agent_data_collection);

        input = (
            "4\n"
            "0 0 0 0 0 0\n"
            "1 10 0 0 0 0\n"
            "2 20 0 0 0 0\n"
            "3 40 0 0 0 0\n"
        );
        input_stream = std::istringstream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        state.update_from_stdin();

        SUBCASE("When agent 0 shoot agent 1")
        {
            soak_overflow::state::Simulator simulator(grid);
            simulator.apply_to(
                state,
                {
                    {0, soak_overflow::action::Shoot(1)}
                }
            );

            SUBCASE("Then agent 1 has 10 wetness")
            {
                CHECK(state.get_agent_collection().get_agent(1).get_wetness() == 10);
            }
        }

        SUBCASE("When agent 0 shoot agent 2")
        {
            soak_overflow::state::Simulator simulator(grid);
            simulator.apply_to(
                state,
                {
                    {0, soak_overflow::action::Shoot(2)}
                }
            );

            SUBCASE("Then agent 2 has 5 wetness")
            {
                CHECK(state.get_agent_collection().get_agent(2).get_wetness() == 5);
            }
        }

        SUBCASE("When agent 0 shoot agent 3")
        {
            soak_overflow::state::Simulator simulator(grid);
            simulator.apply_to(
                state,
                {
                    {0, soak_overflow::action::Shoot(3)}
                }
            );

            SUBCASE("Then agent 3 has 0 wetness")
            {
                CHECK(state.get_agent_collection().get_agent(3).get_wetness() == 0);
            }
        }
    }

    SUBCASE("An agent on target practice with covers and cooldown")
    {
        soak_overflow::grid::Grid grid(10, 3);

        std::string input(
            "4\n"
            "0 0 0 10 10 0\n"
            "1 0 0 10 10 0\n"
            "2 0 0 10 10 0\n"
            "3 0 0 10 10 0\n"
        );
        std::istringstream input_stream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        soak_overflow::agent::AgentDataCollection agent_data_collection(
            soak_overflow::agent::AgentDataCollection::from_stdin()
        );

        soak_overflow::state::State state(&agent_data_collection);

        input = (
            "4\n"
            "0 0 0 0 0 0\n"
            "1 10 0 0 0 0\n"
            "2 20 0 0 0 0\n"
            "3 40 0 0 0 0\n"
        );
        input_stream = std::istringstream(input);
        std::cin.rdbuf(input_stream.rdbuf());

        state.update_from_stdin();
    }
}
