#include <istream>
#include <soak_overflow/agent/AgentCollection.h>

#include <iostream>

namespace soak_overflow::agent
{
    // agent *******************************************************************
    void AgentCollection::crouch_agent(int agent_id)
    {
        __agent_id_to_agent.at(agent_id).crouch();
    }

    const Agent& AgentCollection::get_agent(int agent_id) const
    {
        return __agent_id_to_agent.at(agent_id);
    }

    int AgentCollection::get_agent_id_on(const grid::Coord& cr_coord) const
    {
        return __coord_id_to_agent_id.at(cr_coord.get_id());
    }

    const Agent& AgentCollection::get_agent_on(const grid::Coord& cr_coord) const
    {
        return get_agent(get_agent_id_on(cr_coord));
    }

    const std::unordered_set<int>& AgentCollection::get_agent_ids() const
    {
        return __cp_agent_data_collection->get_agent_ids();
    }

    bool AgentCollection::is_there_an_agent_here(const grid::Coord& cr_coord) const
    {
        return (
            __coord_id_to_agent_id.find(cr_coord.get_id())
                != __coord_id_to_agent_id.end()
        );
    }

    void AgentCollection::move_agent(int agent_id, const grid::Coord& cr_coord)
    {
        Agent& r_agent(__agent_id_to_agent.at(agent_id));
        __coord_id_to_agent_id.erase(r_agent.get_coord().get_id());
        __coord_id_to_agent_id.emplace(cr_coord.get_id(), agent_id);
        r_agent.set_coord(cr_coord);
    }

    void AgentCollection::raise_agent(int agent_id)
    {
        __agent_id_to_agent.at(agent_id).stand_up();
    }

    // agent data **************************************************************
    const AgentDataCollection* AgentCollection::get_agent_data_collection() const
    {
        return __cp_agent_data_collection;
    }

    // constructor *************************************************************
    AgentCollection::AgentCollection(
        const AgentDataCollection* cp_agent_data_collection
    ):
        // agent
        __agent_id_to_agent(),
        __coord_id_to_agent_id(),
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

    AgentCollection::AgentCollection(const AgentCollection& cr_agent_collection):
        // agent
        __agent_id_to_agent(cr_agent_collection.__agent_id_to_agent),
        __coord_id_to_agent_id(cr_agent_collection.__coord_id_to_agent_id),
        // agent data
        __cp_agent_data_collection(cr_agent_collection.__cp_agent_data_collection)
    {
    }

    // update ******************************************************************
    void AgentCollection::update_from_stdin()
    {
        __coord_id_to_agent_id.clear();

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

            grid::Coord agent_coord(x, y);

            __coord_id_to_agent_id.emplace(agent_coord.get_id(), agent_id);
            __agent_id_to_agent.at(agent_id).update(
                agent_coord,
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
