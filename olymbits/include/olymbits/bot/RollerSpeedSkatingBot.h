#ifndef OLYMBITS_BOT_ROLLER_SPEED_SKATTING_BOT_H
#define OLYMBITS_BOT_ROLLER_SPEED_SKATTING_BOT_H

#include <olymbits/bot/Bot.h>

namespace olymbits::bot
{
    class RollerSpeedSkatingBot: public Bot
    {
    // constructor *************************************************************
    public:
        RollerSpeedSkatingBot();

    // play ********************************************************************
    public:
        const action::Action& play(
            const state::State& cr_state,
            int player_index
        );
    };
}

#endif
