#include <olymbits/mini_game/Diving.h>

#include <iostream>

namespace olymbits::mini_game
{
    // constructor *************************************************************
    Diving::Diving():
        MiniGame()
    {
    }

    // diving goal *************************************************************
    char Diving::get_next_action_first_letter() const
    {
        return __gpu[0];
    }

    // simulation **************************************************************
    void Diving::apply_to(
        const action::Action& cr_action_player_0,
        const action::Action& cr_action_player_1,
        const action::Action& cr_action_player_2,
        Diving& r_diving
    ) const
    {
        if (__is_game_over)
        {
            r_diving = *this;
            return;
        }

        char next_action_first_letter(get_next_action_first_letter());

        apply_to(cr_action_player_0, 0, next_action_first_letter, r_diving);
        apply_to(cr_action_player_1, 1, next_action_first_letter, r_diving);
        apply_to(cr_action_player_2, 2, next_action_first_letter, r_diving);

        r_diving.__gpu = __gpu.substr(1);

        if (r_diving.__gpu.size() == 0)
        {
            r_diving.__is_game_over = true;
        }
    }

    void Diving::apply_to(
        const action::Action& cr_action,
        int player_index,
        char next_action_first_letter,
        Diving& r_diving
    ) const
    {
        int player_score_index(player_index);
        int player_combo_index(player_index + 3);

        if (cr_action.get_first_letter() == next_action_first_letter)
        {
            r_diving.__registers[player_combo_index] = (
                __registers[player_combo_index] + 1
            );
        }

        r_diving.__registers[player_score_index] = (
            __registers[player_score_index]
            + r_diving.__registers[player_combo_index]
        );
    }
}
