#ifndef KEEP_OFF_THE_GRASS_ACTION_BUILD_H
#define KEEP_OFF_THE_GRASS_ACTION_BUILD_H

#include <keep_off_the_grass/action/Action.h>

namespace keep_off_the_grass::action
{
    class Build: public Action
    {
    // constructor *************************************************************
    public:
        Build(
            int x,
            int y
        );

    // parameter ***************************************************************
    private:
        int __x;
        
        int __y;

    // stream ******************************************************************
    public:
        void add_to_action_stream(
            std::stringstream& r_action_stream
        ) const override;
    };
}

#endif // KEEP_OFF_THE_GRASS_ACTION_BUILD_H
