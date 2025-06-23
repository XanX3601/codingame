#ifndef OLYMBITS_MCTS_NODE_H
#define OLYMBITS_MCTS_NODE_H

#include <memory>
#include <random>
#include <tuple>
#include <vector>

#include <olymbits/state/State.h>

namespace olymbits::mcts
{
    class Node
    {
    // children ****************************************************************
    private:
        std::vector<std::shared_ptr<Node>> __children;

        std::vector<
            std::tuple<
                std::reference_wrapper<const action::Action>,
                std::reference_wrapper<const action::Action>,
                std::reference_wrapper<const action::Action>
            >
        > __remaining_actions;

        static const std::vector<
            std::tuple<
                std::reference_wrapper<const action::Action>, 
                std::reference_wrapper<const action::Action>, 
                std::reference_wrapper<const action::Action> 
            >
        > __default_remaining_actions;

    // constructor *************************************************************
    public:
        Node();

    // parent ******************************************************************
    private:
        std::shared_ptr<Node> __sp_parent;

    // reset *******************************************************************
    public:
        void reset_but_state();

    private:
        static std::mt19937 __generator;
        static std::random_device __random_device;

    // score *******************************************************************
    private:
        std::vector<int> __scores;

        int __visit_count;

    // state *******************************************************************
    private:
        state::State __state;
    };
}

#endif
