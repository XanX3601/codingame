#include <keep_off_the_grass/action/Build.h>

namespace keep_off_the_grass::action
{
    // constructor *************************************************************
    Build::Build(
        int x,
        int y
    ):
        Action(),
        // parameter
        __x(x),
        __y(y)
    {
    }

    // stream ******************************************************************
    void Build::add_to_action_stream(std::stringstream& r_action_stream) const
    {
        r_action_stream
            << "BUILD "
            << __x << " "
            << __y << ";";
    }
}
