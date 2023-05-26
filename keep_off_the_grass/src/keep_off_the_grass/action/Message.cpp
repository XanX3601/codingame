#include <keep_off_the_grass/action/Message.h>

namespace keep_off_the_grass::action
{
    // constructor *************************************************************
    Message::Message(
        const std::string& cr_content
    ):
        Action(),
        // parameter
        __content(cr_content)
    {
    }

    // stream ******************************************************************
    void Message::add_to_action_stream(std::stringstream& r_action_stream) const
    {
        r_action_stream
            << "MESSAGE "
            << __content << ";";
    }
}
