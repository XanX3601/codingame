#ifndef KEEP_OFF_THE_GRASS_ACTION_MOVE_H
#define KEEP_OFF_THE_GRASS_ACTION_MOVE_H

#include <keep_off_the_grass/action/Action.h>

namespace keep_off_the_grass::action
{
    class Move: public Action
    {
    // constructor *************************************************************
    public:
        Move(
            int unit_count,
            int from_x,
            int from_y,
            int to_x,
            int to_y
        );

    // parameter ***************************************************************
    private:
        int __from_x;

        int __from_y;

        int __to_x;

        int __to_y;

        int __unit_count;

    // stream ******************************************************************
    private:
        void add_to_action_stream(
            std::stringstream& r_action_stream
        ) const override;
    };
}

#endif // KEEP_OFF_THE_GRASS_ACTION_MOVE_H
