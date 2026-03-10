#ifndef SOAK_OVERFLOW_STATE_SIMULATOR_H
#define SOAK_OVERFLOW_STATE_SIMULATOR_H

#include <deque>
#include <unordered_map>
#include <unordered_set>

#include <soak_overflow/action/Combat.h>
#include <soak_overflow/action/Shoot.h>
#include <soak_overflow/action/Throw.h>
#include <soak_overflow/action/Move.h>
#include <soak_overflow/grid/Coord.h>
#include <soak_overflow/grid/Grid.h>
#include <soak_overflow/state/State.h>

namespace soak_overflow::state
{
    class Simulator
    {
    // constructor *************************************************************
    public:
        Simulator(const grid::Grid& cr_grid);

        Simulator(const Simulator& cr_simulator) = delete;

        Simulator(Simulator&& rv_simulator) noexcept = default;

        ~Simulator() noexcept = default;

    // grid ********************************************************************
    private:
        const grid::Grid& __cr_grid;

    // operator ****************************************************************
    public:
        Simulator& operator=(const Simulator& cr_simulator) = delete;

        Simulator& operator=(Simulator&& rv_simulator) noexcept = delete;

    // simulation **************************************************************
    public:
        State apply_to(
            const State& cr_state,
            std::unordered_map<int, const action::Move*> agent_id_to_move
        );

        void apply_to(
            State& r_state,
            std::unordered_map<int, action::Combat> agent_id_to_combat
        );

    private:
        std::deque<int> __agent_ids_queue;

        std::unordered_set<int> __seen_agent_ids;

        std::unordered_set<int> __forbidden_coord_ids;

        void __apply_to(
            State& r_state,
            int agent_id,
            const action::Shoot& cr_shoot
        );

        void __apply_to(
            State& r_state,
            int agent_id,
            const action::Throw& cr_throw
        );
    };
}

#endif
