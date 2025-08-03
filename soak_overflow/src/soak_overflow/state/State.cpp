#include <soak_overflow/state/State.h>

namespace soak_overflow::state
{
    // agent *******************************************************************
    const agent::AgentCollection& State::get_agent_collection() const
    {
        return __agent_collection;
    }

    // constructor *************************************************************
    State::State(
        const agent::AgentDataCollection* cp_agent_data_collection
    ):
        // agent
        __agent_collection(cp_agent_data_collection)
    {
    }

    State::State(const State& cr_state):
        // agent
        __agent_collection(cr_state.__agent_collection)
    {
    }

    // update ******************************************************************
    void State::update_from_stdin()
    {
        __agent_collection.update_from_stdin();
    }
}
