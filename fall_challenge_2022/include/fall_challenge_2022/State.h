#ifndef FALL_CHALLENGE_2022_STATE_H
#define FALL_CHALLENGE_2022_STATE_H

#include <fall_challenge_2022/Map.h>
#include <fall_challenge_2022/Player.h>
#include <unordered_map>

namespace fall_challenge_2022
{
    /**
     * @brief Represents a state of the game.
     */
    class State
    {
    // constructor *************************************************************
    public:
        /**
         * @brief Instantiates this state.
         *
         * @param width the number of column in the map of this state
         * @param height the number of row in the map of this state
         */
        State(int width, int height);

        /**
         * @brief Instantiates this state.
         *
         * @param my_matter the quantity of matter for the player
         * @param opponent_matter the quantity of matter for the opponent
         * @param cr_map_string the string representing the map
         */
        State(
            int my_matter,
            int opponent_matter,
            const std::string& cr_map_string
        );

        /**
         * @brief Instantiates this state by copying another.
         *
         * @param cr_state the stat to copy
         */
        State(const State& cr_state) = default;

        /**
         * @brief Instantiates this state by moving another.
         *
         * @param rv_state the state to move
         */
        State(State&& rv_state) = default;

        /**
         * @brief Destroys this state
         */
        ~State() = default;

    // map *********************************************************************
    public:
        /**
         * @brief Gets the map of this state.
         *
         * @return the map of this state
         */
        const Map& get_map() const;
    
    private:
        /**
         * @__map the map of this state
         */
        Map __map;

    // matter ******************************************************************
    private:
        /**
         * @__player_to_matter associate a player to its amount of matter
         */
        std::unordered_map<Player, int> __player_to_matter;

    // operator ****************************************************************
    public:
        /**
         * @brief Assigns another state to this one.
         *
         * @param cr_state the state to assign
         * @return this state
         */
        State& operator=(const State& cr_state) = default;

        /**
         * @brief Assign another state to this one.
         *
         * @param rv_state the state to assign
         * @return this state
         */
        State& operator=(State&& rv_state) = default;

    // update ******************************************************************
    public:
        /**
         * @brief Updates this state from the standard input
         */
        void update_from_std_in();
    };
}

#endif // FALL_CHALLENGE_2022_STATE_H
