#ifndef KEEP_OFF_THE_GRASS_ACTION_SPAWN_H
#define KEEP_OFF_THE_GRASS_ACTION_SPAWN_H

#include <keep_off_the_grass/action/Action.h>

namespace keep_off_the_grass::action
{
    class Spawn: public Action
    {
    // constructor *************************************************************
    public:
        Spawn(
            int unit_count,
            int x,
            int y
        );

    // parameter ***************************************************************
    private:
        int __unit_count;

        int __x;

        int __y;

    // stream ******************************************************************
    private:
        void add_to_action_stream(
            std::stringstream& r_action_stream
        ) const override;
    };
}

#endif // KEEP_OFF_THE_GRASS_ACTION_SPAWN_H
