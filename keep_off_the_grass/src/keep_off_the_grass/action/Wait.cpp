#include <keep_off_the_grass/action/Wait.h>

namespace keep_off_the_grass::action
{
    // constructor *************************************************************
    Wait::Wait(
    ):
        Action()
    {
    }

    // stream ******************************************************************
    void Wait::add_to_action_stream(std::stringstream& r_action_stream) const
    {
        r_action_stream
            << "WAIT;";
    }
}
