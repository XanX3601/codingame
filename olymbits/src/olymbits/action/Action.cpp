#include <olymbits/action/Action.h>

namespace olymbits::action
{
    // actions *****************************************************************
    Action const Action::UP(Action::up);
    Action const Action::DOWN(Action::down);
    Action const Action::LEFT(Action::left);
    Action const Action::RIGHT(Action::right);

    // constructor *************************************************************
    Action::Action(Value value):
        // value
        __value(value),
        // string
        __first_letter()
    {
        switch (__value)
        {
            case up:
                __first_letter = 'U';
                break;
            case down:
                __first_letter = 'D';
                break;
            case left:
                __first_letter = 'L';
                break;
            case right:
                __first_letter = 'R';
                break;
            default:
                __first_letter = 'X';
        }
    }

    // string ******************************************************************
    char Action::get_first_letter() const
    {
        return __first_letter;
    }

    std::ostream& operator<<(
        std::ostream& r_ostream,
        const Action& cr_action
    )
    {
        r_ostream << cr_action.to_string();
        return r_ostream;
    }

    std::string Action::to_string() const
    {
        switch (__value)
        {
            case up:
                return "UP";
            case down:
                return "DOWN";
            case left:
                return "LEFT";
            case right:
                return "RIGHT";
            default:
                return "UNKNOWN";
        }
    }
}
