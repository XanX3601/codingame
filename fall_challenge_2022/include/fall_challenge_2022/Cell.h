#ifndef FALL_CHALLENGE_2022_CELL_H
#define FALL_CHALLENGE_2022_CELL_H

#include <fall_challenge_2022/Player.h>

namespace fall_challenge_2022
{
    /**
     * @brief Represents a cell of the map.
     */
    class Cell
    {
    // constructor *************************************************************
    public:
        /**
         * @brief Instantiates this map
         */
        Cell();

        /**
         * @brief Instantiates this map by copying another.
         *
         * @param cr_cell the cell to copy
         */
        Cell(const Cell& cr_cell) = default;

        /**
         * @brief Instantiates this cell by moving another.
         *
         * @param rv_cell the cell to move
         */
        Cell(Cell&& rv_cell) = default;

    // info ********************************************************************
    public:
        /**
         * @brief Tests if the player can build a recycler on this cell.
         *
         * @return true if the player can build a recycler on this cell, false
         * otherwise
         */
        bool is_buildable() const;

        /**
         * @brief Tests if this cell is being harvested by a recycler.
         *
         * @return true if this cell is being harvested by a recycler, false
         * otherwise
         */
        bool is_harvested() const;

        /**
         * @brief Tests if the player can spawn units on this cell.
         *
         * @return true if the player can spawn unit on this cell, false
         * otherwise
         */
        bool is_spawnable() const;

        /**
         * @brief Sets wether this cell is buildable or not.
         *
         * @param is_buildable true if the cell is buildable, false otherwise
         */
        void set_buildable(bool is_buildable);

        /**
         * @brief Sets wether this cell is being harvested or not.
         *
         * @param is_harvested true if the cell is being harvested, false
         * otherwise
         */
        void set_harvested(bool is_harvested);

        /**
         * @brief Sets wether this cell is spawnable or not.
         *
         * @param is_spawnable true if the cell is spawnable, false otherwise
         */
        void set_spawnable(bool is_spawnable);
    
    private:
        /**
         * @__is_buildable indicates wether the player can build a recycler on
         * this cell (true) or not (false)
         */
        bool __is_buildable;

        /**
         * @__is_harvested indicates wether this cell is being harvested (true)
         * or not (false)
         */
        bool __is_harvested;

        /**
         * @__is_spawnable indicates wether the player can spawn units on this 
         * cell (true) or not (false)
         */
        bool __is_spawnable;

    // operator ****************************************************************
    public:
        /**
         * @brief Assigns another cell to this one.
         *
         * @param cr_cell the cell to assign
         * @return this cell
         */
        Cell& operator=(const Cell& cr_cell) = default;

        /**
         * @brief Assigns another cell to this one.
         *
         * @param rv_cell the cell to assign
         * @return this cell
         */
        Cell& operator=(Cell&& rv_cell) = default;

    // owner *******************************************************************
    public:
        /**
         * @brief Gets the owner of this cell.
         *
         * @return the owner of this cell
         */
        Player get_owner() const;

        /**
         * @brief Sets the owner of this cell.
         *
         * @param owner the owner of this cell
         */
        void set_owner(Player owner);
    
    private:
        /**
         * @__owner indicates the owner of this cell
         */
        Player __owner;

    // recycler ****************************************************************
    public:
        /**
         * @brief Tests if this cell is occupied by a recycler.
         *
         * @return true if this cell is occupied by a recycler, false otherwise
         */
        bool has_recycler() const;

        /**
         * @brief Sets wether this cell is occupied by a recycler or not.
         *
         * @param has_recycler true if this cell is occupied by a recycler,
         * false otherwise
         */
        void set_recycler(bool has_recycler);
    
    private:
        /**
         * @has_recycler indicates wether this cell is occupied by a recycler
         * (true) or not (false)
         */
        bool __has_recycler;

    // scrap *******************************************************************
    public:
        /**
         * @brief Gets the scrap amount on this cell.
         *
         * @return the scrap amount on this cell
         */
        int get_scrap_amount() const;

        /**
         * @brief Sets the scrap amount on this cell.
         *
         * @param scrap_amount the scrap amount on this cell
         */
        void set_scrap_amount(int scrap_amount);

    private:
        /**
         * @__scrap_amount indicates the amount of scrap on this cell
         */
        int __scrap_amount;

    // unit ********************************************************************
    public:
        /**
         * @brief Gets the amount of unit on this cell.
         *
         * @return the amount of unit on this cell
         */
        int get_unit_amount() const;

        /**
         * @brief Sets the amount of unit on this cell.
         *
         * @param unit_amount the amount of unit on this cell
         */
        void set_unit_amount(int unit_amount);
    
    private:
        /**
         * @__unit_amount the amount of unit on this cell
         */
        int __unit_amount;
    };
}

#endif // FALL_CHALLENGE_2022_CELL_H
