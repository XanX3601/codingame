#ifndef SOAK_OVERFLOW_AGENT_AGENT_DATA_H
#define SOAK_OVERFLOW_AGENT_AGENT_DATA_H

namespace soak_overflow::agent
{
    class AgentData
    {
    // constructor *************************************************************
    public:
        AgentData(
            int id,
            int player_id,
            int shoot_cooldown,
            int optimal_range,
            int soaking_power,
            int initial_splash_bomb_count
        );

        AgentData(const AgentData& cr_agent_data) = default;

        AgentData(AgentData&& rv_agent_data) noexcept = default;

        ~AgentData() noexcept = default;

        static AgentData from_stdin();

    // id **********************************************************************
    public:
        int get_id() const;

    private:
        int __id;

    // operator ****************************************************************
    public:
        AgentData& operator=(const AgentData& cr_agent_data) = default;

        AgentData& operator=(AgentData&& rv_agent_data) noexcept = default;

    // player id ***************************************************************
    public:
        int get_player_id() const;

    private:
        int __player_id;

    // shoot *******************************************************************
    public:
        int get_optimal_range() const;

        int get_shoot_cooldown() const;

        int get_soaking_power() const;

    private:
        int __optimal_range;

        int __shoot_cooldown;

        int __soaking_power;

    // splash bomb *************************************************************
    public:
        int get_initiali_splash_bomb_count() const;

    private:
        int __initial_splash_bomb_count;
    };
}

#endif
