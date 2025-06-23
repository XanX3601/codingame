#ifndef OLYMBITS_MINI_GAME_ROLLER_SPEED_SKATING_H
#define OLYMBITS_MINI_GAME_ROLLER_SPEED_SKATING_H

#include <optional>
#include <random>

#include <olymbits/action/Action.h>
#include <olymbits/mini_game/MiniGame.h>

namespace olymbits::mini_game
{
    class RollerSpeedSkating: public MiniGame
    {
    // actions *****************************************************************
    public:
        const std::vector<
            std::reference_wrapper<const action::Action>
        >& get_actions_ordered_by_risk() const;

    private:
        std::vector<std::reference_wrapper<const action::Action>> __actions;

    // constructor *************************************************************
    public:
        RollerSpeedSkating();

    // risk ********************************************************************
    public:
        std::optional<int> get_player_risk(int player_index) const;

    // simulation **************************************************************
    public:
        void apply_to(
            const action::Action& cr_action_player_0,
            const action::Action& cr_action_player_1,
            const action::Action& cr_action_player_2,
            RollerSpeedSkating& r_roller_speed_skating
        ) const;

    private:
        void apply_to(
            const action::Action& cr_action,
            int player_index,
            RollerSpeedSkating& r_roller_speed_skating
        ) const;

    static std::mt19937 __generator;

    static std::random_device __random_device;

    // update ******************************************************************
    public:
        void update() override;
    };
}

#endif
