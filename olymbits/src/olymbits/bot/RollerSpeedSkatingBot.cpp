#include <olymbits/bot/RollerSpeedSkatingBot.h>

namespace olymbits::bot
{
    // constructor *************************************************************
    RollerSpeedSkatingBot::RollerSpeedSkatingBot():
        Bot()
    {
    }

    // play ********************************************************************
    const action::Action& RollerSpeedSkatingBot::play(
        const state::State& cr_state,
        int player_index
    )
    {
        const mini_game::RollerSpeedSkating& cr_roller_speed_skating(
            cr_state.get_roller_speed_skating()
        );

        if (cr_roller_speed_skating.is_game_over())
        {
            return action::Action::LEFT;
        }

        const std::vector<std::reference_wrapper<const action::Action>>& cr_actions_ordered_by_risk(
            cr_roller_speed_skating.get_actions_ordered_by_risk()
        );

        std::optional<int> optional_player_risk(
            cr_roller_speed_skating.get_player_risk(player_index)
        );
        const int& cr_player_risk(optional_player_risk.value());

        if (cr_player_risk < 0)
        {
            return action::Action::LEFT;
        }
        else if (cr_player_risk == 0)
        {
            return cr_actions_ordered_by_risk[3].get();
        }
        else if (cr_player_risk == 1)
        {
            return cr_actions_ordered_by_risk[0].get();
        }
        else if (
            cr_player_risk == 2
            || cr_player_risk == 3
        )
        {
            return cr_actions_ordered_by_risk[1].get();
        }

        return cr_actions_ordered_by_risk[0].get();
    }
}
