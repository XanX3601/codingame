#ifndef SOAK_OVERFLOW_ACTION_THROW_H
#define SOAK_OVERFLOW_ACTION_THROW_H

#include <soak_overflow/grid/Coord.h>

namespace soak_overflow::action
{
    class Throw
    {
    // constructor *************************************************************
    public:
        Throw(const grid::Coord& cr_coord);

        Throw(const Throw& cr_throw) = default;

        Throw(Throw&& rv_throw) noexcept = default;

        ~Throw() noexcept = default;

    // coord *******************************************************************
    public:
        const grid::Coord& get_coord() const;

    private:
        grid::Coord __coord;

    // operator **************************************************************** 
    public:
        Throw& operator=(const Throw& cr_throw) = default;

        Throw& operator=(Throw&& rv_throw) noexcept = default;
    };
}

#endif
