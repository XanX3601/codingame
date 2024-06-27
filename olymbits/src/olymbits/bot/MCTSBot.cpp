#include <olymbits/bot/MCTSBot.h>

namespace olymbits::bot
{
    // constructor *************************************************************
    MCTSBot::MCTSBot():
        Bot()
    {
    }

    // play ********************************************************************
    const action::Action& MCTSBot::play(
        const state::State& cr_state,
        int player_index
    )
    {
        return action::Action::LEFT;
    }
}
