#ifndef KEEP_OFF_THE_GRASS_BOT_WAIT_BOT_H
#define KEEP_OFF_THE_GRASS_BOT_WAIT_BOT_H

#include <keep_off_the_grass/bot/Bot.h>

namespace keep_off_the_grass::bot
{
    class WaitBot: public Bot
    {
    // constructor *************************************************************
    public:
        WaitBot();

    // play ********************************************************************
    public:
        std::vector<std::unique_ptr<action::Action>> play(
            const state::State& cr_state
        ) override;
    };
}

#endif // KEEP_OFF_THE_GRASS_BOT_WAIT_BOT_H
