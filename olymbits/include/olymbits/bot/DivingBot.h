#ifndef OLYMBITS_BOT_DIVING_BOT_H
#define OLYMBITS_BOT_DIVING_BOT_H

#include <unordered_map>

#include <olymbits/bot/Bot.h>

namespace olymbits::bot
{
    class DivingBot: public Bot
    {
    // actions *****************************************************************
    private:
        std::unordered_map<char, std::reference_wrapper<const action::Action>> __first_letter_to_action;

    // constructor *************************************************************
    public:
        DivingBot();

    // play ********************************************************************
    public:
        const action::Action& play(
            const state::State& cr_state,
            int player_index
        ) override;
    };
}

#endif
