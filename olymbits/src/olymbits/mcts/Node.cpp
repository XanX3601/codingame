#include <olymbits/mcts/Node.h>

#include <algorithm>

namespace olymbits::mcts
{
    // children ****************************************************************
    const std::vector<
        std::tuple<
            std::reference_wrapper<const action::Action>,
            std::reference_wrapper<const action::Action>,
            std::reference_wrapper<const action::Action>
        >
    > Node::__default_remaining_actions = []()
    { 
        std::vector<
            std::tuple<
                std::reference_wrapper<const action::Action>,
                std::reference_wrapper<const action::Action>,
                std::reference_wrapper<const action::Action>
            >
        > default_remaining_actions;

        for (const auto& cr_rw_c_action_player_0: action::Action::actions)
        {
            for (const auto& cr_rw_c_action_player_1: action::Action::actions)
            {
                for (const auto& cr_rw_c_action_player_2: action::Action::actions)
                {
                    default_remaining_actions.emplace_back(
                        cr_rw_c_action_player_0.get(),
                        cr_rw_c_action_player_1.get(),
                        cr_rw_c_action_player_2.get()
                    );
                }
            }
        }

        return default_remaining_actions;
    }();

    // constructor *************************************************************
    Node::Node():
        // children
        __children(),
        __remaining_actions(),
        // parent
        __sp_parent(),
        // score
        __scores(3, 0),
        __visit_count(0),
        // state
        __state()
    {
    }

    // reset *******************************************************************
    void Node::reset_but_state()
    {
        // children
        __children.clear();
        __children.reserve(4);

        __remaining_actions = __default_remaining_actions;
        std::shuffle(
            __remaining_actions.begin(),
            __remaining_actions.end(),
            __generator
        );

        // parent
        __sp_parent.reset();

        // score
        __scores.clear();
        __scores.emplace_back(0);
        __scores.emplace_back(0);
        __scores.emplace_back(0);

        __visit_count = 0;
    }

    // define them together in the right order otherwise, it crashes
    std::random_device Node::__random_device;
    std::mt19937 Node::__generator(__random_device());
}
