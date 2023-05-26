#include <keep_off_the_grass/map/ScoredMap.h>

#include <algorithm>
#include <queue>
#include <unordered_map>

namespace keep_off_the_grass::map
{
    // cell ********************************************************************
    const Cell& ScoredMap::get_max_neighbor(const Cell& cr_cell) const
    {
        const auto& cr_neighbors(cr_cell.get_neighbors());

        auto result = std::max_element(
            cr_neighbors.begin(),
            cr_neighbors.end(),
            [this](
                const std::reference_wrapper<const Cell>& cr_rw_cell_1,
                const std::reference_wrapper<const Cell>& cr_rw_cell_2
            )
            {
                double score_1(__scores[cell_to_index(cr_rw_cell_1.get())]);
                double score_2(__scores[cell_to_index(cr_rw_cell_2.get())]);

                return score_1 < score_2;
            }
        );

        return *result;
    }

    const Cell& ScoredMap::get_max_neighbor(int x, int y) const
    {
        return get_max_neighbor(__cr_map.get_cell(x, y));
    }

    // constructor *************************************************************
    ScoredMap::ScoredMap(
        const Map& cr_map
    ):
        // map
        __cr_map(cr_map),
        // score
        __scores(cr_map.get_cell_count(), 0)
    {
    }

    // score *******************************************************************
    void ScoredMap::add_score(int x, int y, double score)
    {
        __scores[coords_to_index(x, y)] += score;
    }

    void ScoredMap::add_score_and_propagate(
        const Cell& cr_cell,
        double score,
        const std::function<double(double)>& cr_propagate_function
    )
    {
        std::queue<std::reference_wrapper<const Cell>> cell_queue;
        std::unordered_map<int, double> index_to_added_score;

        cell_queue.emplace(cr_cell);

        int cell_index(cell_to_index(cr_cell));
        __scores[cell_index] += score;

        index_to_added_score[cell_index] = score;

        while (!cell_queue.empty())
        {
            const Cell& cr_current_cell(cell_queue.front());
            cell_queue.pop();

            int current_cell_index(cell_to_index(cr_current_cell));
            double score_to_add_to_neighbor(
                cr_propagate_function(index_to_added_score[current_cell_index])
            );

            for (const Cell& cr_neighbor: cr_current_cell.get_neighbors())
            {
                int neighbor_index(cell_to_index(cr_neighbor));

                const auto index_iterator(
                    index_to_added_score.find(neighbor_index)
                );
                if (index_iterator == index_to_added_score.end())
                {
                    __scores[neighbor_index] += score_to_add_to_neighbor;

                    index_to_added_score[neighbor_index] = score_to_add_to_neighbor;
                    cell_queue.emplace(cr_neighbor);
                }
            }
        }
    }

    void ScoredMap::add_score_and_propagate(
        int x,
        int y,
        double score,
        const std::function<double(double)>& cr_propagate_function
    )
    {
        add_score_and_propagate(
            __cr_map.get_cell(x, y),
            score,
            cr_propagate_function
        );
    }

    double ScoredMap::get_score(const Cell& cr_cell) const
    {
        return __scores[cell_to_index(cr_cell)];
    }

    double ScoredMap::get_score(int x, int y) const
    {
        return __scores[coords_to_index(x, y)];
    }

    int ScoredMap::cell_to_index(const Cell& cr_cell) const
    {
        return cr_cell.get_y() * __cr_map.get_width() + cr_cell.get_x();
    }

    int ScoredMap::coords_to_index(int x, int y) const
    {
        return y * __cr_map.get_width() + x;
    }

    // scored map **************************************************************
    ScoredMap ScoredMap::add_scored_map(
        const ScoredMap& cr_scored_map,
        double factor
    )
    {
        ScoredMap result(__cr_map);

        for (auto score_index(0); score_index < __scores.size(); ++score_index)
        {
            result.__scores[score_index] = (
                __scores[score_index]
                + factor * cr_scored_map.__scores[score_index]
            );
        }

        return result;
    }

    void ScoredMap::add_scored_map_in_place(
        const ScoredMap& cr_scored_map,
        double factor
    )
    {
        for (auto score_index(0); score_index < __scores.size(); ++score_index)
        {
            __scores[score_index] += (
                factor *cr_scored_map.__scores[score_index]
            );
        }
    }
}
