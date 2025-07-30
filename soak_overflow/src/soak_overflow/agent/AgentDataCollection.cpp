#include <soak_overflow/agent/AgentDataCollection.h>

#include <iostream>
#include <unordered_set>

namespace soak_overflow::agent
{
    // agent data **************************************************************
    const AgentData& AgentDataCollection::get_agent_data(int agent_id) const
    {
        return __agent_id_to_data.at(agent_id);
    }

    // agent id ****************************************************************
    const std::unordered_set<int>& AgentDataCollection::get_agent_ids() const
    {
        return __agent_ids;
    }

    // constructor *************************************************************
    AgentDataCollection::AgentDataCollection():
        // agent data
        __agent_id_to_data(),
        __agent_ids(),
        // player
        __player_id_to_agent_ids(),
        __player_id_to_enemy_agent_ids()
    {
    }

    AgentDataCollection AgentDataCollection::from_stdin()
    {
        AgentDataCollection agent_data_collection;
    
        int agent_count;

        std::cin >> agent_count;       
        std::cin.ignore();

        // build agent data 
        for (int agent_index(0); agent_index < agent_count; ++agent_index)
        {
            AgentData agent_data(AgentData::from_stdin());

            agent_data_collection.__player_ids.emplace(agent_data.get_player_id());

            agent_data_collection.__agent_ids.emplace(agent_data.get_id());
            agent_data_collection.__agent_id_to_data.emplace(
                agent_data.get_id(),
                std::move(agent_data)
            );
        }

        // build player data
        for (int player_id: agent_data_collection.get_player_ids())
        {
            agent_data_collection.__player_id_to_agent_ids.emplace(
                player_id,
                std::unordered_set<int>()
            );
            agent_data_collection.__player_id_to_enemy_agent_ids.emplace(
                player_id,
                std::unordered_set<int>()
            );
        }

        for (int agent_id: agent_data_collection.get_agent_ids())
        {
            const AgentData& cr_agent_data(agent_data_collection.get_agent_data(agent_id));

            for (int player_id: agent_data_collection.get_player_ids())
            {
                if (cr_agent_data.get_player_id() == player_id)
                {
                    agent_data_collection.__player_id_to_agent_ids[player_id].emplace(agent_id);
                }
                else
                {
                    agent_data_collection.__player_id_to_enemy_agent_ids[player_id].emplace(agent_id);
                }
            }
        }

        return agent_data_collection;
    }

    // player ******************************************************************
    const std::unordered_set<int>& AgentDataCollection::get_player_agent_ids(int player_id) const
    {
        return __player_id_to_agent_ids.at(player_id);
    }

    const std::unordered_set<int>& AgentDataCollection::get_player_enemay_agent_ids(int player_id) const
    {
        return __player_id_to_enemy_agent_ids.at(player_id);
    }

    const std::unordered_set<int>& AgentDataCollection::get_player_ids() const
    {
        return __player_ids;
    }
}
