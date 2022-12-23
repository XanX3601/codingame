#ifndef FALL_CHALLENGE_2022_UNIT_H
#define FALL_CHALLENGE_2022_UNIT_H

#include <fall_challenge_2022/Player.h>

namespace fall_challenge_2022
{
    /**
     * @brief Represents a unit in the game.
     */
    class Unit
    {
    // constructor *************************************************************
    public:
        /**
         * @brief Instantiates this unit.
         */
        Unit(
            int x,
            int y,
            Player owner
        );

        /**
         * @brief Instantiates this unit by copying another.
         *
         * @param cr_unit the unit to copy
         */
        Unit(const Unit& cr_unit) = default;

        /**
         * @brief Instantiates this unit by moving another.
         *
         * @param rv_unit the unit to move
         */
        Unit(Unit&& rv_unit) = default;

        /**
         * @brief Destroys this unit.
         */
        ~Unit() = default;

    // coordinates *************************************************************
    public:
        /**
         * @brief Gets the index of the column on which this unit is.
         *
         * @return the index of the column on which this unit is
         */
        int get_x() const;

        /**
         * @brief Gets the index of the row on which this unit is.
         *
         * @return the index of the row on which this unit is
         */
        int get_y() const;
    
    private:
        /**
         * @x the index of the column on which this unit is
         */
        int __x;

        /**
         * @y the index of the row on which this unit is
         */
        int __y;

    // operator ****************************************************************
    public:
        /**
         * @brief Assigns another unit to this one.
         *
         * @param cr_unit the unit to assign
         * @return this unit
         */
        Unit& operator=(const Unit& cr_unit) = default;

        /**
         * @brief Assigns another unit to this one.
         *
         * @param rv_unit the unit to assign
         * @return this unit
         */
        Unit& operator=(Unit&& rv_unit) = default;

    // owner *******************************************************************
    public:
        /**
         * @brief Gets the owner of this unit.
         *
         * @return the owner of this unit
         */
        Player get_owner() const;
    
    private:
        /**
         * @__owner the owner of this unit
         */
        Player __owner;
    };
}

#endif // FALL_CHALLENGE_2022_UNIT_H
