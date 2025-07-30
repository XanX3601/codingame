#ifndef SOAK_OVERFLOW_AGENT_AGENT_COLLECTION_H
#define SOAK_OVERFLOW_AGENT_AGENT_COLLECTION_H

#include <soak_overflow/agent/Agent.h>
#include <soak_overflow/agent/AgentDataCollection.h>

namespace soak_overflow::agent
{
    class AgentCollection
    {
    // agent *******************************************************************
    public:
        const Agent& get_agent(int agent_id) const;

    private:
        std::unordered_map<int, Agent> __agent_id_to_agent;
    
    // agent data **************************************************************
    private:
        const AgentDataCollection* __cp_agent_data_collection;

    // constructor *************************************************************
    public:
        AgentCollection(
            const AgentDataCollection* cp_agent_data_collection
        );

        AgentCollection(const AgentCollection& cr_agent_collection) = delete;

        AgentCollection(AgentCollection&& rv_agent_collection) noexcept = default;

        ~AgentCollection() noexcept = default;

    // operator ****************************************************************
    public:
        AgentCollection& operator=(const AgentCollection& cr_agent_collection) = delete;

        AgentCollection& operator=(AgentCollection&& rv_agent_collection) noexcept = default;

    // update ******************************************************************
    public:
        void update_from_stdin();
    };
}

#endif

