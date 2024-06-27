#ifndef OLYMBITS_BOT_HEURISTIC_BOT_H
#define OLYMBITS_BOT_HEURISTIC_BOT_H

#include <olymbits/bot/Bot.h>
#include <olymbits/bot/ArcheryBot.h>
#include <olymbits/bot/DivingBot.h>
#include <olymbits/bot/HurdleRaceBot.h>
#include <olymbits/bot/RollerSpeedSkatingBot.h>

namespace olymbits::bot
{
    class HeuristicBot: public Bot
    {
    // constructor *************************************************************
    public:
        HeuristicBot();

    // bots ********************************************************************
    private:
        ArcheryBot __archery_bot;

        DivingBot __diving_bot;

        HurdleRaceBot __hurdle_race_bot;

        RollerSpeedSkatingBot __roller_speed_skating_bot;

    // play ********************************************************************
    public:
        const action::Action& play(
            const state::State& cr_state,
            int player_index
        ) override;
    };
}

#endif
