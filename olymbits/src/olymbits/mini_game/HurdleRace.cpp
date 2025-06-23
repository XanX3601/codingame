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

    // simulation **************************************************************
    void HurdleRace::apply_to(
        const action::Action& cr_action_player_0,
        const action::Action& cr_action_player_1,
        const action::Action& cr_action_player_2,
        HurdleRace& r_hurdle_race
    ) const
    {
        if (__is_game_over)
        {
            r_hurdle_race = *this;
            return;
        }

        r_hurdle_race.__gpu = __gpu;

        apply_to(cr_action_player_0, 0, r_hurdle_race);
        apply_to(cr_action_player_1, 1, r_hurdle_race);
        apply_to(cr_action_player_2, 2, r_hurdle_race);
    }

    void HurdleRace::apply_to(
        const action::Action& cr_action,
        int player_index,
        HurdleRace& r_hurdle_race
    ) const
    {
        r_hurdle_race.__registers[player_index] = __registers[player_index];

        int player_stun_timer_index(player_index + 3);

        if (__registers[player_stun_timer_index] > 0)
        {
            r_hurdle_race.__registers[player_stun_timer_index] = (
                __registers[player_stun_timer_index] - 1
            );
            return;
        }

        bool ignore_hurdle(cr_action == action::Action::UP);
        int speed(
            cr_action == action::Action::LEFT ? 1
            : cr_action == action::Action::RIGHT ? 3
            : 2
        );

        for (int traveled_cell_count(1); traveled_cell_count <= speed; ++traveled_cell_count)
        {
            r_hurdle_race.__registers[player_index]++;

            if (
                !ignore_hurdle
                && r_hurdle_race.__registers[player_index] < __gpu.size()
                && __gpu[r_hurdle_race.__registers[player_index]] == '#'
            )
            {
                r_hurdle_race.__registers[player_stun_timer_index] = 3;
                break;
            }

            ignore_hurdle = false;
        }

        if (r_hurdle_race.__registers[player_index] >= __gpu.size() - 1)
        {
            r_hurdle_race.__is_game_over = true;
        }
    }

    // player ******************************************************************
    int HurdleRace::get_player_position_index(int player_index) const
    {
        return __registers[player_index];
    }
}
