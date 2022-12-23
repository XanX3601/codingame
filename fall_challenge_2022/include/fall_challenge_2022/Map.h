#ifndef FALL_CHALLENGE_2022_MAP_H
#define FALL_CHALLENGE_2022_MAP_H

#include <fall_challenge_2022/Cell.h>
#include <string>
#include <vector>

namespace fall_challenge_2022
{
    /**
     * @brief Represents the map.
     */
    class Map
    {
    // cell ********************************************************************
    public:
        /**
         * @brief Gets the cell on column x and row y.
         *
         * @param x the column index of the cell
         * @param y the row index of the cell
         * @return the cell on column x and row y
         */
        const Cell& get_cell(int x, int y) const;

        /**
         * @brief Gets the cell on column x and row y.
         *
         * @param x the column index of the cell
         * @param y the row index of the cell
         */
        Cell& get_cell(int x, int y);

    private:
        /**
         * @brief Transforms coords (x, y) into their index equivalent.
         *
         * @param x the index of the column
         * @param y the index of the row
         * @return the index of the cell
         */
        int coords_to_index(int x, int y) const;

        /**
         * @__cells the cells composing this map
         */
        std::vector<Cell> __cells;

    // constructor *************************************************************
    public:
        /**
         * @brief Instantiates this map.
         *
         * @param width the number of column in this map
         * @param height the number of row in this map
         */
        Map(int width, int height);

        /**
         * @brief Instantiates this map from a string.
         *
         * The string must respect the format:
         *
         * "{width}x{height}-{cells}"
         *
         * with
         * - {width} (int): the number of column in this map
         * - {height} (int): the number of row in this map
         * - {cells} (string):
         *
         *   "{cell};{cell};..."
         *
         *   where
         *
         *   - {cell} (string):
         *
         *     "{x},{y},{scrap_amount},{owner},{unit_amount},{has_recycler},
         *     {is_buildable},{is_spawnable},{is_harvested}"
         *
         *     where
         *
         *     - {x} (int): the index of the column of the cell
         *     - {y} (int): the index of the row of the cell
         *     - {scrap_amount} (int): the amount of scrap on the cell
         *     - {owner} (int): int representation of the owner of the cell
         *       (see `Player`)
         *     - {unit_amount} (int): the number of unit on the cell
         *     - {has_recycler} (int): 1 if there is a recycler on the cell, 0
         *       otherwise
         *     - {is_buildable} (int): 1 if the player can build a recycler on
         *       the cell, 0 otherwise
         *     - {is_spawnable} (int): 1 if the player can spawn units on the
         *       cell, 0 otherwise
         *     - {is_harvested} (int): 1 if the cell is being harvested by a 
         *       recycler, 0 otherwise
         *
         * @param cr_map_string the map from which instantiate this map
         */
        Map(const std::string& cr_map_string);

        /**
         * @brief Instantiates this map by copying another.
         *
         * @param cr_map the map to copy
         */
        Map(const Map& cr_map) = default;

        /**
         * @brief Instantiates this map by moving another.
         *
         * @param rv_map the map to move
         */
        Map(Map&& rv_map) = default;

        /**
         * @brief Destroys this map
         */
        ~Map() = default;
    
    // dimension ***************************************************************
    public:
        /**
         * @brief Gets the number of row in this map.
         *
         * @return the number of row in this map
         */
        int get_height() const;

        /**
         * @brief Gets the number of column in this map.
         *
         * @return the number of column in this map
         */
        int get_width() const;

    private:
        /**
         * @__height the number of row in this map
         */
        int __height;

        /**
         * @__width the number of column in this map
         */
        int __width;

    // operator ****************************************************************
    public:
        /**
         * @brief Assigns another map to this one.
         *
         * @param cr_map the map to assign
         * @return this map
         */
        Map& operator=(const Map& cr_map) = default;

        /**
         * @brief Assigns another map to this one.
         *
         * @param rv_map the map to assign
         * @return this map
         */
        Map& operator=(Map&& rv_map) = default;

    // string ******************************************************************
    public:
        /**
         * @brief Computes a string representation of this map.
         *
         * @return this map as a string
         */
        std::string to_string() const;
    };
}

#endif // FALL_CHALLENGE_2022_MAP_H
