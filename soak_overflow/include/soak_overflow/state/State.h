#ifndef SOAK_OVERFLOW_STATE_STATE_H
#define SOAK_OVERFLOW_STATE_STATE_H

#include <soak_overflow/agent/AgentCollection.h>

namespace soak_overflow::state
{
    class State
    {
    // agent *******************************************************************
    public:
        const agent::AgentCollection& get_agent_collection() const;

    private:
        agent::AgentCollection __agent_collection;

    // constructor *************************************************************
    public:
        State(
            const agent::AgentDataCollection* cp_agent_data_collection
        );
        
        State(const State& cr_state);

        State(State&& rv_state) noexcept = default;

        ~State() noexcept = default;

    // operator ****************************************************************
    public:
        State& operator=(const State& cr_state) = delete;

        State& operator=(State&& rv_state) noexcept = default;

    // simulation
    friend class Simulator;

    // update ******************************************************************
    public:
        void update_from_stdin();
    };
}

#endif
