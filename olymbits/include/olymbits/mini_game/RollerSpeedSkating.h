#ifndef OLYMBITS_MINI_GAME_ROLLER_SPEED_SKATING_H
#define OLYMBITS_MINI_GAME_ROLLER_SPEED_SKATING_H

#include <optional>

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

    // update ******************************************************************
    public:
        void update() override;
    };
}

#endif
