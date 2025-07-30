#include <soak_overflow/agent/AgentData.h>

#include <iostream>

namespace soak_overflow::agent
{
    // constructor *************************************************************
    AgentData::AgentData(
        int id,
        int player_id,
        int shoot_cooldown,
        int optimal_range,
        int soaking_power,
        int initial_splash_bomb_count
    ):
        // id
        __id(id),
        // player id
        __player_id(player_id),
        // shoot
        __shoot_cooldown(shoot_cooldown),
        __optimal_range(optimal_range),
        __soaking_power(soaking_power),
        // splash bomb
        __initial_splash_bomb_count(initial_splash_bomb_count)
    {
    }

    AgentData AgentData::from_stdin() 
    {
        int id;
        int player_id;
        int shoot_cooldown;
        int optimal_range;
        int soaking_power;
        int initial_splash_bomb_count;

        std::cin
            >> id
            >> player_id
            >> shoot_cooldown
            >> optimal_range
            >> soaking_power
            >> initial_splash_bomb_count;
        std::cin.ignore();

        return AgentData(
            id,
            player_id,
            shoot_cooldown,
            optimal_range,
            soaking_power,
            initial_splash_bomb_count
        );
    }

    // id **********************************************************************
    int AgentData::get_id() const
    {
        return __id;
    }

    // playaer id **************************************************************
    int AgentData::get_player_id() const
    {
        return __player_id;
    }

    // shoot *******************************************************************
    int AgentData::get_optimal_range() const
    {
        return __optimal_range;
    }

    int AgentData::get_shoot_cooldown() const
    {
        return __shoot_cooldown;
    }

    int AgentData::get_soaking_power() const
    {
        return __soaking_power;
    }

    // splash bomb *************************************************************
    int AgentData::get_initiali_splash_bomb_count() const
    {
        return __initial_splash_bomb_count;
    }
}
