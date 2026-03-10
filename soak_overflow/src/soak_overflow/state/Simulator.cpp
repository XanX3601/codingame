#include "soak_overflow/action/Shoot.h"
#include "soak_overflow/grid/Cell.h"
#include "soak_overflow/grid/Direction.h"
#include <soak_overflow/action/Move.h>
#include <soak_overflow/agent/Agent.h>
#include <soak_overflow/agent/AgentCollection.h>
#include <soak_overflow/grid/CellType.h>
#include <soak_overflow/grid/Coord.h>
#include <soak_overflow/state/Simulator.h>

#include <iostream>
#include <variant>

namespace soak_overflow::state
{
    // constructor *************************************************************
    Simulator::Simulator(const grid::Grid& cr_grid):
        // grid
        __cr_grid(cr_grid),
        // simulation
        __agent_ids_queue(),
        __forbidden_coord_ids(),
        __seen_agent_ids()
    {
    }

    // simulation **************************************************************
    State Simulator::apply_to(
        const State& cr_current_state,
        std::unordered_map<int, const action::Move*> agent_id_to_move
    )
    {
        // clear tools
        __agent_ids_queue.clear();
        __forbidden_coord_ids.clear();
        __seen_agent_ids.clear();

        // create next state as copy of current one
        State next_state(cr_current_state);
        agent::AgentCollection& r_next_agent_collection(
            next_state.get_agent_collection()
        );

        // put all agent in a queue
        for (const auto& [cr_agent_id, cp_move]: agent_id_to_move)
        {
            __agent_ids_queue.emplace_back(cr_agent_id);
        }

        while (!__agent_ids_queue.empty())
        {
            int agent_id(__agent_ids_queue.front());
            __agent_ids_queue.pop_front();

            const action::Move* cp_move(agent_id_to_move[agent_id]);

            // # CROUCH / STAY
            if (cp_move == &action::Move::stay)
            {
                r_next_agent_collection.crouch_agent(agent_id);
            }
            else
            {
                r_next_agent_collection.raise_agent(agent_id);
            }

            // # MOVE
            grid::Coord next_coord(cp_move->apply_to(
                cr_current_state.get_agent_collection().get_agent(agent_id).get_coord()
            ));

            // ignore move if
            // - the coord is forbidden
            // - the coord is not in the grid
            // - the corresponding cell is not empty
            if (
                __forbidden_coord_ids.find(next_coord.get_id())
                    != __forbidden_coord_ids.cend()
                || !__cr_grid.is_in_grid(next_coord)
                || __cr_grid.get_cell(next_coord).get_type() != grid::CellType::empty
            )
            {
                continue;
            }

            // move agent if there is not agent on next cell
            if (!r_next_agent_collection.is_there_an_agent_here(next_coord))
            {
                r_next_agent_collection.move_agent(agent_id, next_coord);
                continue;
            }

            // wait for the agent on next cell to move
            if (__seen_agent_ids.find(agent_id) == __seen_agent_ids.cend())
            {
                __seen_agent_ids.emplace(agent_id);
                __agent_ids_queue.emplace_back(agent_id);
                continue;
            }

            // the next cell is forbidden has this agent cannot go on it so it
            // makes futur check faster
            __forbidden_coord_ids.emplace(next_coord.get_id());

            // if agent on next cell was there last turn, let it in peace
            if (
                cr_current_state.get_agent_collection().is_there_an_agent_here(next_coord)
                && cr_current_state.get_agent_collection().get_agent_id_on(next_coord)
                    == r_next_agent_collection.get_agent_id_on(next_coord)
            )
            {
                continue;
            }

            // agent on next cell was not there last turn, move it back to its
            // previous cell
            int other_agent_id(
                r_next_agent_collection.get_agent_id_on(next_coord)
            );
            r_next_agent_collection.move_agent(
                other_agent_id,
                cr_current_state.get_agent_collection().get_agent(other_agent_id).get_coord()
            );
        }

        return next_state;
    }

    void Simulator::apply_to(
        State& r_state,
        std::unordered_map<int, action::Combat> agent_id_to_combat
    )
    {
        for (const auto& [cr_agent_id, cr_combat]: agent_id_to_combat)
        {
            if (std::holds_alternative<action::Shoot>(cr_combat))
            {
                __apply_to(
                    r_state,
                    cr_agent_id,
                    std::get<action::Shoot>(cr_combat)
                );
            }
            else
            {
                __apply_to(
                    r_state,
                    cr_agent_id,
                    std::get<action::Throw>(cr_combat)
                );
            }
        }
    }

    void Simulator::__apply_to(
        State& r_state,
        int agent_id,
        const action::Shoot& cr_shoot
    )
    {
        const agent::Agent& cr_agent(r_state.get_agent_collection().get_agent(agent_id));
        const agent::Agent& cr_target(r_state.get_agent_collection().get_agent(cr_shoot.get_target_id()));

        // compute distance between agent and target
        int distance(cr_agent.get_coord().distance_to(cr_target.get_coord()));

        int range(cr_agent.get_agent_data()->get_optimal_range());

        // if target is too far, nothing happens
        if (distance > 2 * range)
        {
            return;
        }
        
        // shot is valid, compute the damage
        int damage(cr_agent.get_agent_data()->get_soaking_power());
        if (distance > range)
        {
            damage /= 2;
        }

        // now damage reduction
        // compute the direction from target to agent to check if neighbor cell
        // is not empty
        double damage_ratio(1);

        grid::Direction direction_target_to_agent(
            cr_target.get_coord().compute_orthogonal_direction_to(
                cr_agent.get_coord()
            )
        );
        grid::Coord cover_coord(
            cr_target.get_coord().apply_direction(direction_target_to_agent)
        );
        const grid::Cell& cr_cover_cell(__cr_grid.get_cell(cover_coord));

        if (cr_cover_cell.get_type() == grid::CellType::low_crate)
        {
            damage_ratio = .5;
        }
        else if (cr_cover_cell.get_type() == grid::CellType::high_crate)
        {
            damage_ratio = .25;
        }

        if (cr_target.is_crouched())
        {
            damage_ratio -= .25;
        }

        damage = damage * damage_ratio;

        r_state.get_agent_collection().shoot_agent(
            cr_shoot.get_target_id(), damage
        );
    }

    void Simulator::__apply_to(
        State& r_state,
        int agent_id,
        const action::Throw& cr_throw
    )
    {
    }
}
