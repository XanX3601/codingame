#ifndef SOAK_OVERFLOW_AGENT_AGENT_DATA_COLLECTION_H
#define SOAK_OVERFLOW_AGENT_AGENT_DATA_COLLECTION_H

#include <unordered_map>
#include <unordered_set>

#include <soak_overflow/agent/AgentData.h>

namespace soak_overflow::agent
{
    class AgentDataCollection
    {
    // agent *******************************************************************
    public:
        const AgentData& get_agent_data(int agent_id) const;

        const std::unordered_set<int>& get_agent_ids() const;

    private:
        std::unordered_map<int, AgentData> __agent_id_to_data;

        std::unordered_set<int> __agent_ids;

    // constructor *************************************************************
    public:
        AgentDataCollection();

        AgentDataCollection(const AgentDataCollection& cr_agent_data_collection) = delete;

        AgentDataCollection(AgentDataCollection&& rv_agent_data_collection) noexcept = default;

        ~AgentDataCollection() noexcept = default;

        static AgentDataCollection from_stdin();

    // operator ****************************************************************
    public:
        AgentDataCollection& operator=(const AgentDataCollection& cr_agent_data_collection) = delete;

        AgentDataCollection& operator=(AgentDataCollection&& rv_agent_data_collection) noexcept = default;

    // player ******************************************************************
    public:
        const std::unordered_set<int>& get_player_agent_ids(int player_id) const;

        const std::unordered_set<int>& get_player_enemay_agent_ids(int player_id) const;

        const std::unordered_set<int>& get_player_ids() const;

    private:
        std::unordered_map<int, std::unordered_set<int>> __player_id_to_agent_ids;

        std::unordered_map<int, std::unordered_set<int>> __player_id_to_enemy_agent_ids;

        std::unordered_set<int> __player_ids;
    };
}

#endif
