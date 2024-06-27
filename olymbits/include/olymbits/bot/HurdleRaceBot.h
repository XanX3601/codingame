#ifndef OLYMBITS_BOT_HURDLE_RACE_BOT_H
#define OLYMBITS_BOT_HURDLE_RACE_BOT_H

#include <olymbits/bot/Bot.h>

namespace olymbits::bot
{
    class HurdleRaceBot: public Bot
    {
    // constructor *************************************************************
    public:
        HurdleRaceBot();

    // play ********************************************************************
    public:
        const action::Action& play(
            const state::State& cr_state,
            int index_player
        );
    };
}

#endif
