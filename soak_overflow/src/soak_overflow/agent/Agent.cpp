#include <soak_overflow/agent/Agent.h>

namespace soak_overflow::agent
{
    // constructor *************************************************************
    Agent::Agent(
        const AgentData* cp_data
    ):
        // coord
        __coord(),
        // data
        __cp_data(cp_data),
        // shoot
        __current_shoot_cooldown(0),
        // splash bomb
        __current_splash_bomb_count(0),
        // state
        __alive(true),
        // wetness
        __wetness(0)
    {
    }

    // coord *******************************************************************
    const grid::Coord& Agent::get_coord() const
    {
        return __coord;
    }

    // shoot *******************************************************************
    int Agent::get_current_shoot_cooldown() const
    {
        return __current_shoot_cooldown;
    }

    // splash bomb *************************************************************
    int Agent::get_current_splash_bomb_count() const
    {
        return __current_splash_bomb_count;
    }

    // state *******************************************************************
    bool Agent::is_alive() const
    {
        return __alive;
    }

    void Agent::kill()
    {
        __alive = false;
    }

    // update ******************************************************************
    void Agent::update(
        grid::Coord coord,
        int current_shoot_cooldown,
        int current_splash_bomb_count,
        int wetness
    )
    {
        __coord = coord;
        __current_shoot_cooldown = current_shoot_cooldown;
        __current_splash_bomb_count = current_splash_bomb_count;
        __wetness = wetness;
    }

    // wetness *****************************************************************
    int Agent::get_wetness() const
    {
        return __wetness;
    }
}
