#ifndef OLYMBITS_BOT_ARCHERY_BOT_H
#define OLYMBITS_BOT_ARCHERY_BOT_H

#include <olymbits/bot/Bot.h>

namespace olymbits::bot
{
    class ArcheryBot: public Bot
    {
    // constructor *************************************************************
    public:
        ArcheryBot();

    // play ******************************************************************** 
    public:
        const action::Action& play(
            const state::State& cr_state,
            int player_index
        );
    };
}

#endif
