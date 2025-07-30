import enum
import math
import sys

class Agent:
    def __init__(
        self,
        id,
        player_id,
        shoot_cooldown,
        optimal_range,
        soaking_power,
        splash_bombs
    ):
        self.id = id
        self.player_id = player_id
        self.shoot_cooldown = shoot_cooldown
        self.optimal_range = optimal_range
        self.soaking_power = soaking_power
        self.splash_bombs = splash_bombs

        self.x, self.y = None, None
        self.cooldown = None
        self.splash_bomb_count = None
        self.wetness = None
        self.alive = True

    @staticmethod
    def from_input():
        return Agent(*(map(int, input().split())))

class AgentCollection:
    def __init__(self):
        self.agent_id_to_agent = {}
        self.player_id_to_agent_ids = {}
        self.xy_to_agent_id = {}

    def add_agent(self, agent):
        self.agent_id_to_agent[agent.id] = agent
        if agent.player_id not in self.player_id_to_agent_ids:
            self.player_id_to_agent_ids[agent.player_id] = set()

        self.player_id_to_agent_ids[agent.player_id].add(agent.id)

    @staticmethod
    def from_input():
        agent_collection = AgentCollection()

        agent_count = int(input())
        for _ in range(agent_count):
            agent_collection.add_agent(Agent.from_input())

        return agent_collection

    def get_agent(self, agent_id):
        return self.agent_id_to_agent[agent_id]

    def get_agent_id_on_tile(self, x, y):
        return self.xy_to_agent_id.get((x, y), None)

    def get_enemy_agent_ids(self, player_id):
        agent_ids = set()
        for p_id, a_ids in self.player_id_to_agent_ids.items():
            if p_id == player_id:
                continue

            agent_ids = agent_ids | a_ids

        return agent_ids

    def get_player_agent_ids(self, player_id):
        return self.player_id_to_agent_ids[player_id]

    def update_from_input(self):
        self.xy_to_agent_id = {}

        dead_agent_ids = set(self.agent_id_to_agent.keys())

        remaining_agent_count = int(input())
        for _ in range(remaining_agent_count):
            (
                agent_id,
                x, y,
                cooldown,
                splash_bomb_count,
                wetness,
            ) = map(int, input().split())

            dead_agent_ids.remove(agent_id)

            agent = self.agent_id_to_agent[agent_id]
            agent.x = x
            agent.y = y
            agent.cooldown = cooldown
            agent.splash_bomb_count = splash_bomb_count
            agent.wetness = wetness

            self.xy_to_agent_id[(agent.x, agent.y)] = agent.id

        input() # my remaining agent count

        for dead_agent_id in dead_agent_ids:
            dead_agent = self.agent_id_to_agent[dead_agent_id]
            dead_agent.alive = False

class TileType(enum.Enum):
    empty = 0
    low_cover = 1
    high_cover = 2

class Tile:
    __next_id = 0

    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.neighbors = []
        self.orthogonal_neighbors = []
        self.type = TileType.empty

        self.id = Tile.__next_id
        Tile.__next_id += 1

    def __lt__(self, other_tile):
        if not isinstance(other_tile, Tile):
            raise TypeError(f"not supported operation '<' between instance of Tile and {type(other_tile)}")

        return self.id < other_tile.id

class TileMap:
    def __init__(self, width, height):
        self.width = width
        self.height = height

        self.xy_to_tile = {}

        for x in range(width):
            for y in range(height):
                self.xy_to_tile[(x, y)] = Tile(x, y)

        for tile in self.xy_to_tile.values():
            x, y = tile.x, tile.y

            for dx, dy in [(1, 0), (-1, 0), (0, 1), (0, -1)]:
                neighbor_x = x + dx
                neighbor_y = y + dy

                neighbor_xy = (neighbor_x, neighbor_y)
                if neighbor_xy in self.xy_to_tile:
                    neighbor = self.xy_to_tile[neighbor_xy]
                    tile.orthogonal_neighbors.append(neighbor)
                    tile.neighbors.append(neighbor)

            for dx, dy in [(1, 1), (-1, 1), (1, -1), (-1, -1)]:
                neighbor_x = x + dx
                neighbor_y = y + dy

                neighbor_xy = (neighbor_x, neighbor_y)
                if neighbor_xy in self.xy_to_tile:
                    neighbor = self.xy_to_tile[neighbor_xy]
                    tile.neighbors.append(neighbor)

    def compute_manhattan_distance(self, x0, y0, x1, y1):
        return abs(x0 - x1) + abs(y0 - y1)

    def compute_line(self, x0, y0, x1, y1):
        def compute_vertical_line(x0, y0, x1, y1):
            reverse = False
            if y0 > y1:
                x0, x1 = x1, x0
                y0, y1 = y1, y0
                reverse = True

            delta_x = x1 - x0
            delta_y = y1 - y0

            direction = -1 if delta_x < 0 else 1
            delta_x *= direction

            assert delta_y != 0

            line = []
            x = x0
            increase_x_factor = 2 * delta_x - delta_y

            for step_index in range(delta_y + 1):
                line.append((x, y0 + step_index))
                if increase_x_factor >= 0:
                    x += direction
                    increase_x_factor -= 2 * delta_y
                increase_x_factor += 2 * delta_x

            if reverse:
                line = line[::-1]

            return line

        def compute_horizontal_line(x0, y0, x1, y1):
            reverse = False
            if x0 > x1:
                x0, x1 = x1, x0
                y0, y1 = y1, y0
                reverse = True

            delta_x = x1 - x0
            delta_y = y1 - y0

            direction = -1 if delta_y < 0 else 1
            delta_y *= direction

            assert delta_x != 0

            line = []
            y = y0
            increase_y_factor = 2 * delta_y - delta_x

            for step_index in range(delta_x + 1):
                line.append((x0 + step_index, y))
                if increase_y_factor >= 0:
                    y += direction
                    increase_y_factor -= 2 * delta_x
                increase_y_factor += 2 * delta_y

            if reverse:
                line = line[::-1]

            return line

        if abs(x1 - x0) > abs(y1 - y0):
            return compute_horizontal_line(x0, y0, x1, y1)
        return compute_vertical_line(x0, y0, x1, y1)

    def compute_orthogonally_direction(self, x0, y0, x1, y1):
        delta_x = x1 - x0
        delta_y = y1 - y0

        distance = math.hypot(delta_x, delta_y)

        if distance <= 0:
            return 0, 0

        delta_x = delta_x / distance
        delta_y = delta_y / distance

        if abs(delta_x) > abs(delta_y):
            return int(round(delta_x)), 0
        return 0, int(round(delta_y))

    def debug(self):
        for y in range(self.height):
            tile_types = []
            for x in range(self.width):
                tile = self.xy_to_tile[(x, y)]
                tile_types.append(tile.type.value)

            print(tile_types, file=sys.stderr)

    def get_tile(self, x, y):
        return self.xy_to_tile[(x, y)]

    def update_from_input(self):
        for y in range(self.height):
            line = list(map(int, input().split()))

            for x in range(self.width):
                tile_type = TileType(line[3 * x + 2])
                self.xy_to_tile[(x, y)].type = tile_type

class TileMapMask:
    def __init__(self, tile_map):
        self.tile_map = tile_map

        self.xy_to_value = {}
        self.max_value = None
        self.max_xy = None

    def debug(self):
        for y in range(self.tile_map.height):
            values = []

            for x in range(self.tile_map.width):
                values.append(f"{self.xy_to_value.get((x, y), 0):.2f}")

            print(values, file=sys.stderr)

    def fill(self, value):
        self.xy_to_value = {
            (x, y): value
            for x in range(self.tile_map.width)
            for y in range(self.tile_map.height)
        }

    def get_value(self, x, y):
        return self.xy_to_value.get((x, y), 0)

    def set_value(self, x, y, value):
        self.xy_to_value[(x, y)] = value

        if self.max_value is None or value > self.max_value:
            self.max_value = value
            self.max_xy = (x, y)

def get_damage_mask(tile_map, agent_collection, agent_id):
    mask = TileMapMask(tile_map)

    agent = agent_collection.get_agent(agent_id)
    soaking_power = agent.soaking_power
    enemy_agent_ids = agent_collection.get_enemy_agent_ids(agent.player_id)

    for enemy_agent_id in enemy_agent_ids:
        enemy_agent = agent_collection.get_agent(enemy_agent_id)
        enemy_tile = tile_map.get_tile(enemy_agent.x, enemy_agent.y)

        tiles = [enemy_tile] + enemy_tile.orthogonal_neighbors

        for tile in tiles:
            direction_x, direction_y = tile_map.compute_orthogonally_direction(
                tile.x,
                tile.y,
                agent.x,
                agent.y
            )

            neighbor_x = tile.x + direction_x
            neighbor_y = tile.y + direction_y

            neighbor = tile_map.get_tile(neighbor_x, neighbor_y)

            value = soaking_power
            if neighbor.type == TileType.low_cover:
                value *= .5
            elif neighbor.type == TileType.high_cover:
                value *= .25

            if tile_map.compute_manhattan_distance(
                tile.x, tile.y, agent.x, agent.y
            ) > agent.optimal_range:
                value = 0

            mask.set_value(tile.x, tile.y, value)

    return mask

def get_splash_damage_mask(tile_map, agent_collection, player_id):
    mask = TileMapMask(tile_map)

    enemy_agent_ids = agent_collection.get_enemy_agent_ids(player_id)
    tiles = []
    for enemy_agent_id in enemy_agent_ids:
        enemy_agent = agent_collection.get_agent(enemy_agent_id)
        tile = tile_map.get_tile(enemy_agent.x, enemy_agent.y)

        tiles = tiles + [tile] + tile.orthogonal_neighbors

    for tile in tiles:
        splash_ally = False

        value = 30
        for neighbor in tile.neighbors:
            agent_id = agent_collection.get_agent_id_on_tile(
                neighbor.x,
                neighbor.y
            )
            if agent_id is not None:
                agent = agent_collection.get_agent(agent_id)

                value += 30
                if agent.player_id == player_id:
                    splash_ally = True

        if splash_ally: 
            value = 0

        mask.set_value(tile.x, tile.y, value)

    return mask

def main():
    # init 
    # initialization input
    my_id = int(input())

    agent_collection = AgentCollection.from_input()

    width, height = map(int, input().split())

    tile_map = TileMap(width, height)
    tile_map.update_from_input()

    my_agent_ids = list(agent_collection.get_player_agent_ids(my_id))
    enemy_agent_ids = list(agent_collection.get_enemy_agent_ids(my_id))

    # game loop
    while True:
        # turn input
        agent_collection.update_from_input()

        splash_mask = get_splash_damage_mask(tile_map, agent_collection, my_id)
        target_xy = splash_mask.max_xy
        target_x, target_y = target_xy

        for my_agent_id in my_agent_ids:
            print(f"{my_agent_id}; MOVE {target_x} {target_y}; THROW {target_x} {target_y}")

if __name__ == "__main__":
    main()
