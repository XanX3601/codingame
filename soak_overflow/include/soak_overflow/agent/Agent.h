#ifndef SOAK_OVERFLOW_AGENT_AGENT_H
#define SOAK_OVERFLOW_AGENT_AGENT_H

#include <soak_overflow/agent/AgentData.h>
#include <soak_overflow/grid/Coord.h>

namespace soak_overflow::agent
{
    class Agent
    {
    // constructor *************************************************************
    public:
        Agent(
            const AgentData* cp_data
        );
    
        Agent(const Agent& cr_agent);

        Agent(Agent&& rv_agent) noexcept = default;

        ~Agent() noexcept = default;

    // coord *******************************************************************
    public:
        const grid::Coord& get_coord() const;

        void set_coord(const grid::Coord& cr_coord);

    private:
        grid::Coord __coord;

    // crouch ******************************************************************
    public:
        void crouch();

        bool is_crouched() const;

        void stand_up();

    private:
        bool __crouched;

    // data ********************************************************************
    public:
        const AgentData* get_agent_data() const;

    private:
        const AgentData* __cp_data;

    // operator ****************************************************************
    public:
        Agent& operator=(const Agent& cr_agent) = delete;

        Agent& operator=(Agent&& rv_agent) noexcept = default;

    // shoot *******************************************************************
    public:
        int get_current_shoot_cooldown() const;

    private:
        int __current_shoot_cooldown;

    // splash bomb *************************************************************
    public:
        int get_current_splash_bomb_count() const;

    private:
        int __current_splash_bomb_count;

    // state *******************************************************************
    public:
        bool is_alive() const;

        void kill();

    private:
        bool __alive;

    // update ******************************************************************
    public:
        void update(
            grid::Coord coord,
            int current_shoot_cooldown,
            int current_splash_bomb_count,
            int wetness
        );

    // wetness *****************************************************************
    public:
        int get_wetness() const;

    private:
        int __wetness;
    };
}

#endif
