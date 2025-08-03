#include <soak_overflow/agent/Agent.h>

namespace soak_overflow::agent
{
    // constructor *************************************************************
    Agent::Agent(
        const AgentData* cp_data
    ):
        // coord
        __coord(),
        // crouch
        __crouched(false),
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

    Agent::Agent(
        const Agent& cr_agent
    ):
        // coord
        __coord(cr_agent.__coord),
        // crouch
        __crouched(cr_agent.__crouched),
        // data
        __cp_data(cr_agent.__cp_data),
        // shoot
        __current_shoot_cooldown(cr_agent.__current_shoot_cooldown),
        // splash bomb
        __current_splash_bomb_count(cr_agent.__current_splash_bomb_count),
        // state
        __alive(cr_agent.__alive),
        // wetness
        __wetness(cr_agent.__wetness)
    {
    }

    // coord *******************************************************************
    const grid::Coord& Agent::get_coord() const
    {
        return __coord;
    }

    void Agent::set_coord(const grid::Coord& cr_coord)
    {
        __coord = cr_coord;
    }

    // crouch ******************************************************************
    void Agent::crouch()
    {
        __crouched = true;
    }

    bool Agent::is_crouched() const
    {
        return __crouched;
    }

    void Agent::stand_up()
    {
        __crouched = false;
    }

    // data ********************************************************************
    const AgentData* Agent::get_agent_data() const
    {
        return __cp_data;
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
