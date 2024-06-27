#include <olymbits/mini_game/RollerSpeedSkating.h>

#include <algorithm>

namespace olymbits::mini_game
{
    // actions *****************************************************************
    const std::vector<std::reference_wrapper<const action::Action>>& RollerSpeedSkating::get_actions_ordered_by_risk() const
    {
        return __actions;
    }

    // constructor *************************************************************
    RollerSpeedSkating::RollerSpeedSkating():
        MiniGame(),
        // actions 
        __actions{
            action::Action::UP,
            action::Action::DOWN,
            action::Action::LEFT,
            action::Action::RIGHT
        }
    {
    }

    // risk ********************************************************************
    std::optional<int> RollerSpeedSkating::get_player_risk(
        int player_index
    ) const
    {
        if (is_game_over())          
        {
            return std::nullopt;
        }

        return __registers[3 + player_index];
    }

    // update ******************************************************************
    void RollerSpeedSkating::update()
    {
        MiniGame::update();

        if (is_game_over())
        {
            return;
        }

        std::sort(
            __actions.begin(),
            __actions.end(),
            [&](
                std::reference_wrapper<const action::Action> rw_c_action_a, 
                std::reference_wrapper<const action::Action> rw_c_action_b
            )
            {
                std::string::size_type position_a(
                    __gpu.find(rw_c_action_a.get().get_first_letter())
                );
                std::string::size_type position_b(
                    __gpu.find(rw_c_action_b.get().get_first_letter())
                );

                return position_a < position_b;
            }
        );
    }
}
