#include <istream>
#include <soak_overflow/agent/AgentCollection.h>

#include <iostream>

namespace soak_overflow::agent
{
    // agent *******************************************************************
    const Agent& AgentCollection::get_agent(int agent_id) const
    {
        return __agent_id_to_agent.at(agent_id);
    }

    // constructor *************************************************************
    AgentCollection::AgentCollection(
        const AgentDataCollection* cp_agent_data_collection
    ):
        // agent
        __agent_id_to_agent(),
        // agent data
        __cp_agent_data_collection(cp_agent_data_collection)
    {
        for (int agent_id: cp_agent_data_collection->get_agent_ids())
        {
            __agent_id_to_agent.emplace(
                agent_id,
                std::move(Agent(&(cp_agent_data_collection->get_agent_data(agent_id))))
            );
        }
    }

    // update ******************************************************************
    void AgentCollection::update_from_stdin()
    {
        int remaining_agent_count;

        std::cin >> remaining_agent_count;
        std::cin.ignore();

        std::unordered_set<int> remaining_agent_ids;

        for (int agent_index(0); agent_index < remaining_agent_count; ++agent_index)
        {
            int agent_id;
            int x;
            int y;
            int current_shoot_cooldown;
            int current_splash_bomb_count;
            int wetness;

            std::cin
                >> agent_id
                >> x
                >> y
                >> current_shoot_cooldown
                >> current_splash_bomb_count
                >> wetness;
            std::cin.ignore();

            remaining_agent_ids.emplace(agent_id);

            __agent_id_to_agent.at(agent_id).update(
                {x, y},
                current_shoot_cooldown,
                current_splash_bomb_count,
                wetness
            );
        }

        for (int agent_id: __cp_agent_data_collection->get_agent_ids())
        {
            if (remaining_agent_ids.find(agent_id) == remaining_agent_ids.end())
            {
                __agent_id_to_agent.at(agent_id).kill();
            }
        }
    }
}
