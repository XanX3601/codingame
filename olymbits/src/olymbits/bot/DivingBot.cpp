#include <olymbits/bot/DivingBot.h>

#include <iostream>

namespace olymbits::bot
{
    // constructor *************************************************************
    DivingBot::DivingBot():
        Bot(),
        // actions
        __first_letter_to_action{
            {action::Action::UP.get_first_letter(), action::Action::UP},
            {action::Action::DOWN.get_first_letter(), action::Action::DOWN},
            {action::Action::LEFT.get_first_letter(), action::Action::LEFT},
            {action::Action::RIGHT.get_first_letter(), action::Action::RIGHT},
        }
    {
    }

    // play ********************************************************************
    const action::Action& DivingBot::play(
        const state::State& cr_state,
        int player_index
    )
    {
        const mini_game::Diving& cr_diving(cr_state.get_diving());

        if (cr_diving.is_game_over())
        {
            return action::Action::RIGHT;
        }

        char next_action_first_letter(cr_diving.get_next_action_first_letter());

        return __first_letter_to_action.at(next_action_first_letter).get();
    }
}
