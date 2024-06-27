#ifndef OLYMBITS_MCTS_MCTS_H
#define OLYMBITS_MCTS_MCTS_H

#include <olymbits/state/State.h>

namespace olymbits::mcts
{
    class MCTS
    {
    // compute *****************************************************************
    public:
        void compute(const state::State& cr_state, int player_index);

    // constructor *************************************************************
    public:
        MCTS();
    };
}

#endif
