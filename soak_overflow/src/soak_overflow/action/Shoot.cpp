#include <soak_overflow/action/Shoot.h>

namespace soak_overflow::action
{
    // constructor *************************************************************
    Shoot::Shoot(int target_id):
        // target
        __target_id(target_id)
    {
    }

    // target ******************************************************************
    int Shoot::get_target_id() const
    {
        return __target_id;
    }
}
