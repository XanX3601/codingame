#include <olymbits/mini_game/HurdleRace.h>

namespace olymbits::mini_game
{
    // constructor *************************************************************
    HurdleRace::HurdleRace():
        MiniGame()
    {
    }

    // hurdle ******************************************************************
    std::optional<int> HurdleRace::get_distance_to_next_hurdle(int position_index) const
    {
        if (is_game_over())
        {
            return std::nullopt;
        }

        std::optional<int> optional_distance_to_next_hurdle;
        for (
            int next_position_index(position_index + 1);
            next_position_index < __gpu.size();
            ++next_position_index
        )
        {
            if (__gpu[next_position_index] == '#')
            {
                optional_distance_to_next_hurdle = (
                    next_position_index - position_index
                );
                break;
            }
        }

        return optional_distance_to_next_hurdle;
    }

    // player ******************************************************************
    int HurdleRace::get_player_position_index(int player_index) const
    {
        return __registers[player_index];
    }
}
