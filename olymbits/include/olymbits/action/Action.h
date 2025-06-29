#ifndef OLYMBITS_ACTION_ACTION_H
#define OLYMBITS_ACTION_ACTION_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace olymbits::action
{
    class Action
    {
    // value *******************************************************************
    // must be kept on top of the class
    public:
        enum Value
        {
            up,
            down,
            left,
            right
        };

    private:
        Value __value;

    // actions *****************************************************************
    public:
        static const Action UP;
        static const Action DOWN;
        static const Action LEFT;
        static const Action RIGHT;

        static const std::vector<std::reference_wrapper<const Action>> actions;

    // constructor *************************************************************
    public:
        Action(Value value);

    // operator ****************************************************************
    public:
        bool operator==(const Action& cr_action) const;

    // string ******************************************************************
    public:
        char get_first_letter() const;

        friend std::ostream& operator<<(
            std::ostream& r_ostream,
            const Action& cr_action
        );

        std::string to_string() const;

    private:
        char __first_letter;
    };
}

#endif
