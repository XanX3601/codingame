#include <olymbits/bot/HurdleRaceBot.h>

namespace olymbits::bot
{
    // constructor *************************************************************
    HurdleRaceBot::HurdleRaceBot():
        Bot()
    {
    }

    // play ********************************************************************
    const action::Action& HurdleRaceBot::play(
        const state::State& cr_state,
        int player_index
    )
    {
        const mini_game::HurdleRace& cr_hurdle_race(cr_state.get_hurdle_race());

        int player_position_index(
            cr_hurdle_race.get_player_position_index(player_index)
        );
        std::optional<int> optional_distance_to_next_hurdle(
            cr_hurdle_race.get_distance_to_next_hurdle(player_position_index)
        );

        if (!optional_distance_to_next_hurdle.has_value())
        {
            return action::Action::RIGHT;
        }

        const int& cr_distance_to_next_hurdle(
            optional_distance_to_next_hurdle.value()
        );

        if (cr_distance_to_next_hurdle <= 1)
        {
            return action::Action::UP;
        }
        else if (cr_distance_to_next_hurdle <= 2)
        {
            return action::Action::LEFT;
        }
        else if (cr_distance_to_next_hurdle <= 3)
        {
            return action::Action::DOWN;
        }
        
        return action::Action::RIGHT;
    }
}
