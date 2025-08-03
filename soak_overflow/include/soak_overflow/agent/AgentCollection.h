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
        void crouch_agent(int agent_id);

        const Agent& get_agent(int agent_id) const;

        int get_agent_id_on(const grid::Coord& cr_coord) const;

        const Agent& get_agent_on(const grid::Coord& cr_coord) const;

        const std::unordered_set<int>& get_agent_ids() const;

        bool is_there_an_agent_here(const grid::Coord& cr_coord) const;

        void move_agent(int agent_id, const grid::Coord& cr_coord);

        void raise_agent(int agent_id);

    private:
        std::unordered_map<int, Agent> __agent_id_to_agent;

        std::unordered_map<int, int> __coord_id_to_agent_id;
    
    // agent data **************************************************************
    public:
        const AgentDataCollection* get_agent_data_collection() const;

    private:
        const AgentDataCollection* __cp_agent_data_collection;

    // constructor *************************************************************
    public:
        AgentCollection(
            const AgentDataCollection* cp_agent_data_collection
        );

        AgentCollection(const AgentCollection& cr_agent_collection);

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

