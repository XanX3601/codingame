#include <olymbits/mini_game/RollerSpeedSkating.h>

#include <algorithm>
#include <memory>

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

    // simulation **************************************************************
    void RollerSpeedSkating::apply_to(
        const action::Action& cr_action_player_0,
        const action::Action& cr_action_player_1,
        const action::Action& cr_action_player_2,
        RollerSpeedSkating& r_roller_speed_skating
    ) const
    {
        if (__is_game_over)
        {
            r_roller_speed_skating = *this;
            return;
        }

        apply_to(cr_action_player_0, 0, r_roller_speed_skating);
        apply_to(cr_action_player_1, 1, r_roller_speed_skating);
        apply_to(cr_action_player_2, 2, r_roller_speed_skating);

        if (
            r_roller_speed_skating.__registers[0] == r_roller_speed_skating.__registers[1]
            || r_roller_speed_skating.__registers[0] == r_roller_speed_skating.__registers[2]
        )
        {
            r_roller_speed_skating.__registers[3] += 2;
        }
        if (
            r_roller_speed_skating.__registers[1] == r_roller_speed_skating.__registers[0]
            || r_roller_speed_skating.__registers[1] == r_roller_speed_skating.__registers[2]
        )
        {
            r_roller_speed_skating.__registers[4] += 2;
        }
        if (
            r_roller_speed_skating.__registers[2] == r_roller_speed_skating.__registers[0]
            || r_roller_speed_skating.__registers[2] == r_roller_speed_skating.__registers[1]
        )
        {
            r_roller_speed_skating.__registers[5] += 2;
        }

        if (r_roller_speed_skating.__registers[3] >= 5)
        {
            r_roller_speed_skating.__registers[3] = -2;
        }
        if (r_roller_speed_skating.__registers[4] >= 5)
        {
            r_roller_speed_skating.__registers[4] = -2;
        }
        if (r_roller_speed_skating.__registers[5] >= 5)
        {
            r_roller_speed_skating.__registers[5] = -2;
        }

        std::shuffle(
            r_roller_speed_skating.__actions.begin(),
            r_roller_speed_skating.__actions.end(),
            __generator
        );
    }

    void RollerSpeedSkating::apply_to(
        const action::Action& cr_action,
        int player_index,
        RollerSpeedSkating& r_roller_speed_skating
    ) const
    {
        int player_position_index(player_index);
        int player_risk_index(player_index + 3);

        if (__registers[player_risk_index] < 0)
        {
            r_roller_speed_skating.__registers[player_position_index] = (
                __registers[player_position_index]
            );
            r_roller_speed_skating.__registers[player_risk_index] = (
                __registers[player_risk_index] + 1
            );
            return;
        }

        auto iterator = std::find(__actions.begin(), __actions.end(), cr_action);
        int action_index(iterator - __actions.begin());

        if (action_index == 0)
        {
            r_roller_speed_skating.__registers[player_position_index] = (
                __registers[player_position_index] + 1
            );
            r_roller_speed_skating.__registers[player_risk_index] = std::max(
                __registers[player_risk_index] - 1,
                0
            );
        }
        else if (action_index == 1)
        {
            r_roller_speed_skating.__registers[player_position_index] = (
                __registers[player_position_index] + 2
            );
            r_roller_speed_skating.__registers[player_risk_index] = (
                __registers[player_risk_index]
            );
        }
        else if (action_index == 2)
        {
            r_roller_speed_skating.__registers[player_position_index] = (
                __registers[player_position_index] + 2
            );
            r_roller_speed_skating.__registers[player_risk_index] = (
                __registers[player_risk_index] + 1
            );
        }
        else
        {
            r_roller_speed_skating.__registers[player_position_index] = (
                __registers[player_position_index] + 3
            );
            r_roller_speed_skating.__registers[player_risk_index] = (
                __registers[player_risk_index] + 2
            );
        }
    }

    std::mt19937 RollerSpeedSkating::__generator(__random_device());

    std::random_device RollerSpeedSkating::__random_device;

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
