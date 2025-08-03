#ifndef SOAK_OVERFLOW_ACTION_MOVE_H
#define SOAK_OVERFLOW_ACTION_MOVE_H

#include <array>

#include <soak_overflow/grid/Coord.h>
#include <soak_overflow/grid/Direction.h>

namespace soak_overflow::action
{
    class Move
    {
    // apply *******************************************************************
    public:
        grid::Coord apply_to(const grid::Coord& cr_coord) const;

    // constructor *************************************************************
    public:
        Move(grid::Direction direction);

        Move(const Move& cr_move) = default;

        Move(Move&& rv_move) noexcept = default;

        ~Move() noexcept = default;

    // direction ***************************************************************
    public:
        const grid::Direction& get_direction() const;

    private:
        grid::Direction __direction;

    // moves *******************************************************************
    public:
        static const Move down;

        static const Move left;

        static const std::array<const Move, 5> moves;

        static const Move right;

        static const Move stay;

        static const Move up;
    
    // operator ****************************************************************
    public:
        Move& operator=(const Move& cr_move) = delete;

        Move& operator=(Move&& rv_move) noexcept = delete;

        bool operator==(const Move& cr_move) const;
    };
}

#endif
