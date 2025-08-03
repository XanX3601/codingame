#ifndef SOAK_OVERFLOW_ACTION_COMBAT_H
#define SOAK_OVERFLOW_ACTION_COMBAT_H

#include <soak_overflow/grid/Coord.h>

namespace soak_overflow::action
{
    class Combat
    {
    // constructor *************************************************************
    public:
        Combat(
            grid::Coord coord,
            bool is_thrown
        );
    
        Combat(const Combat& cr_combat) = delete;

        Combat(Combat&& rv_combat) noexcept = default;

        ~Combat() noexcept = default;

    // coord *******************************************************************
    public:
        const grid::Coord& get_coord() const;

    private:
        grid::Coord __coord;

    // operator ****************************************************************
    public:
        Combat& operator=(const Combat& cr_combat) = delete;

        Combat& operator=(Combat&& rv_combat) noexcept = delete;

    // throw *******************************************************************
    public:
        bool is_thrown() const;

    private:
        bool __throw;
    };
}

#endif
