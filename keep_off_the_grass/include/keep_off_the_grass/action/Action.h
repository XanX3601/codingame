#ifndef KEEP_OFF_THE_GRASS_ACTION_ACTION_H
#define KEEP_OFF_THE_GRASS_ACTION_ACTION_H

#include <sstream>

namespace keep_off_the_grass::action
{
    class Action
    {
    // constructor *************************************************************
    public:
        Action(
        );

        virtual ~Action() = default;

    // stream ******************************************************************
    public:
        virtual void add_to_action_stream(
            std::stringstream& r_action_stream
        ) const = 0;
    };
}

#endif // KEEP_OFF_THE_GRASS_ACTION_ACTION_H
