#include <soak_overflow/state/Simulator.h>

#include <iostream>

namespace soak_overflow::state
{
    // constructor *************************************************************
    Simulator::Simulator(const grid::Grid& cr_grid):
        // grid
        __cr_grid(cr_grid),
        // simulation
        __agent_ids_deque(),
        __agent_ids_set()
    {
    }

    // simulation **************************************************************
    State Simulator::apply_to(
        const State& cr_current_state,
        std::unordered_map<int, const action::Move*> agent_id_to_move
    )
    {
        // TODO: check if clear is necessary
        __agent_ids_set.clear();
        __agent_ids_deque.clear();

        State next_state(cr_current_state);

        // put alive agent in the queue
        for (const auto& [agent_id, move]: agent_id_to_move)
        {
            if (!cr_current_state.get_agent_collection().get_agent(agent_id).is_alive())
            {
                continue;
            }
            __agent_ids_deque.emplace_back(agent_id);
        }

        // now move agent one by one, if one tries to move on another agent that
        // is still in the queue
        while (!__agent_ids_deque.empty())
        {
            // take the first agent id in the queue
            int agent_id(__agent_ids_deque.front());
            __agent_ids_deque.pop_front();

            // raise the current agent
            next_state.__agent_collection.raise_agent(agent_id);

            // retrieve the move for the current agent
            const action::Move* cp_move(agent_id_to_move.at(agent_id));

            // if the agent stays, it crouches
            if (cp_move == &action::Move::stay)
            {
                next_state.__agent_collection.crouch_agent(agent_id);
                continue;
            }

            // compute the current agent next coord
            grid::Coord next_agent_coord(
                cp_move->apply_to(
                    cr_current_state
                        .__agent_collection
                        .get_agent(agent_id)
                        .get_coord()
                )
            );

            // first check for legal move according to the grid
            // if the next cell is not empty or is not in the grid, move is
            // illegal
            bool next_cell_in_grid(__cr_grid.is_in_grid(next_agent_coord));
            if (!next_cell_in_grid)
            {
                continue;
            }

            bool next_cell_empty(
                __cr_grid.get_cell(next_agent_coord).get_type() == grid::CellType::empty
            );
            if (!next_cell_empty)
            {
                continue;
            }

            // second check for legal move according to agent collection
            // if there is no agent on next cell, move the agent
            bool is_there_an_agent_on_next_cell(
                next_state.__agent_collection.is_there_an_agent_here(next_agent_coord)
            );
            if (!is_there_an_agent_on_next_cell)
            {
                next_state.__agent_collection.move_agent(
                    agent_id,
                    next_agent_coord
                );
                continue;
            }

            // now we know there is another agent on the next cell
            // this other agent may be
            //   - moving out of the next cell on this turn
            //   - was on the next cell previous turn and stay this turn
            //   - moved to next cell on this turn
            // if the current agent is seen for the first time in the loop, put
            // it back in the queue to let the other agent have a chance to 
            // move out of the next cell
            bool has_agent_already_been_seen(
                __agent_ids_set.find(agent_id) != __agent_ids_set.end()
            );
            if (! has_agent_already_been_seen)
            {
                __agent_ids_set.emplace(agent_id);
                __agent_ids_deque.emplace_back(agent_id);
                continue;
            }

            // the other agent did not move out of the next cell
            // if it was there previous turn, then do nothing
            bool was_other_agent_already_on_next_cell(
                cr_current_state.__agent_collection.is_there_an_agent_here(next_agent_coord)
                && (
                    cr_current_state.__agent_collection.get_agent_id_on(next_agent_coord)
                        == next_state.__agent_collection.get_agent_id_on(next_agent_coord)
                )
            );
            if (was_other_agent_already_on_next_cell)
            {
                continue;
            }

            // the other agent moved to next cell on this turn, so agent and
            // other agent bump into each other and cancel their move
            // so bring back other agent to his previous coord
            int other_agent_id(
                next_state.__agent_collection.get_agent_id_on(next_agent_coord)
            );
            next_state.__agent_collection.move_agent(
                other_agent_id,
                cr_current_state.__agent_collection.get_agent(other_agent_id).get_coord()
            );
        }

        return next_state;
    }
}
