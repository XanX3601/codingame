#ifndef OLYMBITS_BOT_MCTS_BOT_H
#define OLYMBITS_BOT_MCTS_BOT_H

#include <olymbits/bot/Bot.h>

namespace olymbits::bot
{
    class MCTSBot: public Bot
    {
    // constructor *************************************************************
    public:
        MCTSBot();

    // play ********************************************************************
    public:
        const action::Action& play(
            const state::State& cr_state,
            int player_index
        ) override;
    };
}

#endif
