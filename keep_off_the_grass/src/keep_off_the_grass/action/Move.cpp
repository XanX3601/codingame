#include <keep_off_the_grass/action/Move.h>

namespace keep_off_the_grass::action
{
    // constructor *************************************************************
    Move::Move(
        int unit_count,
        int from_x,
        int from_y,
        int to_x,
        int to_y
    ):
        Action(),
        // parameter
        __from_x(from_x),
        __from_y(from_y),
        __to_x(to_x),
        __to_y(to_y),
        __unit_count(unit_count)
    {
    }

    // stream ******************************************************************
    void Move::add_to_action_stream(std::stringstream& r_action_stream) const
    {
        r_action_stream
            << "MOVE "
            << __unit_count << " "
            << __from_x << " "
            << __from_y << " "
            << __to_x << " "
            << __to_y << ";";
    }
}
