import collections
import dataclasses
import enum
import math
import sys


class CellType(enum.Enum):
    empty = 0
    egg = 1
    crystal = 2


@dataclasses.dataclass
class CellData:
    resource_count: int
    type: CellType


class Map:
    def __init__(self):
        self.cell_indexes = set()
        self.cell_index_to_data = {}
        self.cell_index_to_neighbors = {}
        self.crystal_indexes = set()

    def add_cell(self, cell_index, cell_data, cell_neighbors):
        self.cell_indexes.add(cell_index)
        self.cell_index_to_data[cell_index] = cell_data
        self.cell_index_to_neighbors[cell_index] = cell_neighbors

        if cell_data.type == CellType.crystal:
            self.crystal_indexes.add(cell_index)

    def cell_count(self):
        return len(self.cell_indexes)

    def cell_data(self, cell_index):
        return self.cell_index_to_data[cell_index]

    def cell_neighbor_indexes(self, cell_index):
        return self.cell_index_to_neighbors[cell_index]

    def cell_set_resource_count(self, cell_index, resource_count):
        self.cell_index_to_data[cell_index].resource_count = resource_count

    @staticmethod
    def instantiate_from_input():
        map = Map()

        cell_count = int(input())
        for cell_index in range(cell_count):
            data = input().split()
            cell_type = CellType(int(data[0]))
            cell_resource_count = int(data[1])
            cell_data = CellData(cell_resource_count, cell_type)
            cell_neighbors = {int(n) for n in data[2:] if int(n) >= 0}
            map.add_cell(cell_index, cell_data, cell_neighbors)

        return map


def compute_crystal_score(
    crystal_index,
    crystal_index_to_nearest_my_base_index,
    my_base_index_to_cell_index_to_distance_from_base,
    my_ant_count,
    map,
):
    nearest_my_base_index = crystal_index_to_nearest_my_base_index[crystal_index]
    distance_from_nearest_my_base = my_base_index_to_cell_index_to_distance_from_base[
        nearest_my_base_index
    ][crystal_index]
    mining_speed = math.floor(my_ant_count / distance_from_nearest_my_base)
    crystal_count = map.cell_data(crystal_index).resource_count
    harvest_time = math.ceil(crystal_count / mining_speed)
    score = crystal_count / harvest_time if harvest_time > 0 else 0
    return score


def compute_distance_to_each_cell(map, root_index):
    queue = collections.deque()
    queue.appendleft(root_index)

    cell_index_to_parent_index = {root_index: None}
    cell_index_to_distance_from_root = {root_index: 0}

    while queue:
        current_cell_index = queue.pop()
        current_cell_distance = cell_index_to_distance_from_root[current_cell_index]

        for neighbor_index in map.cell_neighbor_indexes(current_cell_index):
            if neighbor_index in cell_index_to_parent_index:
                continue

            cell_index_to_parent_index[neighbor_index] = current_cell_index
            cell_index_to_distance_from_root[neighbor_index] = current_cell_distance + 1

            queue.appendleft(neighbor_index)

    return cell_index_to_parent_index, cell_index_to_distance_from_root

def compute_path(from_index, to_index, cell_index_to_parent_index):
    path = collections.deque()

    current_cell_index = to_index
    while current_cell_index is not from_index:
        path.appendleft(current_cell_index)
        current_cell_index = cell_index_to_parent_index[current_cell_index]
    path.appendleft(from_index)

    return path
    
# main *************************************************************************
map = Map.instantiate_from_input()

base_count = int(input())
my_base_indexes = {int(i) for i in input().split()}
opponent_base_indexes = {int(i) for i in input().split()}

# compute distance from base to all cells
my_base_index_to_cell_index_to_parent_index = {}
my_base_index_to_cell_index_to_distance_from_base = {}
for my_base_index in my_base_indexes:
    (
        cell_index_to_parent_index,
        cell_index_to_distance_from_base,
    ) = compute_distance_to_each_cell(map, my_base_index)
    my_base_index_to_cell_index_to_parent_index[
        my_base_index
    ] = cell_index_to_parent_index
    my_base_index_to_cell_index_to_distance_from_base[
        my_base_index
    ] = cell_index_to_distance_from_base

# compute nearest base from each crysal
crystal_index_to_nearest_my_base_index = {
    crystal_index: min(
        my_base_indexes,
        key=lambda my_base_index: my_base_index_to_cell_index_to_distance_from_base[
            my_base_index
        ][crystal_index],
    )
    for crystal_index in map.crystal_indexes
}

crystal_indexes = list(map.crystal_indexes)

turn_index = 0
my_ant_count = 0

crystal_goal_index = None

while True:
    # update from input
    for cell_index in range(map.cell_count()):
        data = input().split()
        cell_resource_count = int(data[0])
        cell_my_ant_count = int(data[1])
        cell_opponent_ant_count = int(data[1])

        map.cell_set_resource_count(cell_index, cell_resource_count)

        if turn_index == 0:
            my_ant_count += cell_my_ant_count

    # reset crystal goal if it has been harvested
    if (
        crystal_goal_index is not None
        and map.cell_data(crystal_goal_index).resource_count == 0
    ):
        crystal_goal_index = None

    # compute crystal goal if necessary
    if crystal_goal_index is None:
        crystal_indexes.sort(
            key=lambda crystal_index: compute_crystal_score(
                crystal_index,
                crystal_index_to_nearest_my_base_index,
                my_base_index_to_cell_index_to_distance_from_base,
                my_ant_count,
                map,
            ),
            reverse=True,
        )
        crystal_goal_index = crystal_indexes[0]

    # retrieve nearest base from crystal goal
    nearest_my_base_index = crystal_index_to_nearest_my_base_index[crystal_goal_index]

    # compute path from crystal goal to nearest base
    path = compute_path(nearest_my_base_index, crystal_goal_index, my_base_index_to_cell_index_to_parent_index[nearest_my_base_index])

    # map path to beacon
    orders = [f"BEACON {cell_index} 1" for cell_index in path]
    orders = ";".join(orders)

    print(orders)

    turn_index += 1
