#include <keep_off_the_grass/action/Spawn.h>

namespace keep_off_the_grass::action
{
    // constructor *************************************************************
    Spawn::Spawn(
        int unit_count,
        int x,
        int y
    ):
        Action(),
        // parameter
        __unit_count(unit_count),
        __x(x),
        __y(y)
    {
    }
    
    // stream ******************************************************************
    void Spawn::add_to_action_stream(std::stringstream& r_action_stream) const
    {
        r_action_stream
            << "SPAWN "
            << __x << " "
            << __y << ";";
    }
}
