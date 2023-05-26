#ifndef KEEP_OFF_THE_GRASS_ACTION_MESSAGE_H
#define KEEP_OFF_THE_GRASS_ACTION_MESSAGE_H

#include <string>

#include <keep_off_the_grass/action/Action.h>

namespace keep_off_the_grass::action
{
    class Message: public Action
    {
    // constructor *************************************************************
    public:
        Message(
            const std::string& cr_content
        );

    // parameter ***************************************************************
    private:
        std::string __content;

    // stream ******************************************************************
    public:
        void add_to_action_stream(
            std::stringstream& r_action_stream
        ) const override;
    };
}

#endif // KEEP_OFF_THE_GRASS_ACTION_MESSAGE_H
