#ifndef KEEP_OFF_THE_GRASS_ACTION_WAIT_H
#define KEEP_OFF_THE_GRASS_ACTION_WAIT_H

#include <keep_off_the_grass/action/Action.h>

namespace keep_off_the_grass::action
{
    class Wait: public Action
    {
    // constructor *************************************************************
    public:
        Wait(
        );

    // stream ******************************************************************
    public:
        void add_to_action_stream(
            std::stringstream& r_action_stream
        ) const override;
    };
}

#endif // KEEP_OFF_THE_GRASS_ACTION_WAIT_H
