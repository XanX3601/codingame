import dataclasses

@dataclasses.dataclass
class Creature:
    id: int
    color: int
    type: int
    x: int = 0
    y: int = 0
    vx: int = 0
    vy: int = 0

@dataclasses.dataclass
class Drone:
    id: int
    x: int = 0
    y: int = 0
    battery: int = 0
    scanned_creature_ids: set = dataclasses.field(default_factory=set)
    radar_to_creature_ids: dict = dataclasses.field(
        default_factory=lambda: {
            "TL": set(),
            "TR": set(),
            "BR": set(),
            "BL": set()
        }
    )
    creature_id_to_radar: dict = dataclasses.field(default_factory=dict)

class Bot:
    def __init__(self):
        # play
        self.turn_index = 0

        # data 
        self.creature_id_to_creature = {}
        self.color_to_creature_ids = {}
        self.drone_id_to_drone = {}
        self.foe_drone_ids = set()
        self.foe_scanned_creature_ids = set()
        self.my_drone_ids = set()
        self.my_scanned_creature_ids = set()
        self.visible_creature_ids = set()

        # strategy
        self.colors = []

    def handle_init_input(self):
        creature_count = int(input())

        for _ in range(creature_count):
            creature_id, creature_color, creature_type = map(int, input().split())

            self.creature_id_to_creature[creature_id] = Creature(
                id=creature_id,
                color=creature_color,
                type=creature_type
            )

            if creature_color not in self.color_to_creature_ids:
                self.color_to_creature_ids[creature_color] = set()
            self.color_to_creature_ids[creature_color].add(creature_id)

    def handle_turn_input(self):
        my_score = int(input())
        foe_score = int(input())

        my_scan_count = int(input())
        for _ in range(my_scan_count):
            creature_id = int(input())

            self.my_scanned_creature_ids.add(creature_id)

        foe_scan_count = int(input())
        for _ in range(foe_scan_count):
            creature_id = int(input())

            self.foe_scanned_creature_ids.add(creature_id)

        my_drone_count = int(input())
        for _ in range(my_drone_count):
            drone_id, drone_x, drone_y, _, drone_battery = map(int, input().split())

            if drone_id not in self.drone_id_to_drone:
                self.drone_id_to_drone[drone_id] = Drone(id=drone_id)
                self.my_drone_ids.add(drone_id)

            drone = self.drone_id_to_drone[drone_id]
            drone.x = drone_x
            drone.y = drone_y
            drone.battery = drone_battery

            drone.scanned_creature_ids.clear()
            for creature_ids in drone.radar_to_creature_ids.values():
                creature_ids.clear()

        foe_drone_count = int(input())
        for _ in range(foe_drone_count):
            drone_id, drone_x, drone_y, _, drone_battery = map(int, input().split())

            if drone_id not in self.drone_id_to_drone:
                self.drone_id_to_drone[drone_id] = Drone(id=drone_id)
                self.foe_drone_ids.add(drone_id)

            drone = self.drone_id_to_drone[drone_id]
            drone.x = drone_x
            drone.y = drone_y
            drone.battery = drone_battery

            drone.scanned_creature_ids.clear()
            for creature_ids in drone.radar_to_creature_ids.values():
                creature_ids.clear()

        drone_scan_count = int(input())
        for _ in range(drone_scan_count):
            drone_id, creature_id = map(int, input().split())

            drone = self.drone_id_to_drone[drone_id]
            drone.scanned_creature_ids.add(creature_id)

        visible_creature_count = int(input())
        for _ in range(visible_creature_count):
            creature_id, creature_x, creature_y, creature_vx, creature_vy = map(int, input().split())

            creature = self.creature_id_to_creature[creature_id]
            creature.x = creature_x
            creature.y = creature_y
            creature.vx = creature_vx
            creature.vy = creature_vy

        radar_blip_count = int(input())
        for _ in range(radar_blip_count):
            drone_id, creature_id, radar = input().split()
            drone_id = int(drone_id)
            creature_id = int(creature_id)

            drone = self.drone_id_to_drone[drone_id]
            drone.radar_to_creature_ids[radar].add(creature_id)
            drone.creature_id_to_radar[creature_id] = radar

    def play(self):
        self.handle_init_input()

        while True:
            self.handle_turn_input()

            if self.turn_index == 0:
                self.colors = list(self.color_to_creature_ids.keys())

            my_drone_id = list(self.my_drone_ids)[0]
            my_drone = self.drone_id_to_drone[my_drone_id]

            color_index = 0
            found_color = False
            while not found_color:
                color = self.colors[color_index]
                creature_ids = self.color_to_creature_ids[color]

                if not all(id in self.my_scanned_creature_ids for id in creature_ids):
                    found_color = True
                else:
                    color_index += 1
                    if color_index >= len(self.colors):
                        break

            if not found_color:
                self.wait()
                continue

            color = self.colors[color_index]
            creature_ids = list(self.color_to_creature_ids[color])

            creature_id_index = 0
            found_creature_id = False
            while not found_creature_id:
                creature_id = creature_ids[creature_id_index]

                if (
                    creature_id not in my_drone.scanned_creature_ids
                    and creature_id not in self.my_scanned_creature_ids
                ):
                    found_creature_id = True
                else:
                    creature_id_index += 1
                    if creature_id_index >= len(creature_ids):
                        break

            if not found_creature_id:
                self.move_to_surface(my_drone)
                continue

            creature_id = creature_ids[creature_id_index]

            if creature_id in self.visible_creature_ids:
                creature = self.creature_id_to_creature[creature_id]
                self.move_to(creature.x, creature.y)
                continue

            radar = my_drone.creature_id_to_radar[creature_id]
            self.move_to_radar(radar)

            self.turn_index += 1

    def wait(self, light=False):
        print(f"WAIT {'1' if light else '0'}")

    def move_to_surface(self, drone, light=False):
        print(f"MOVE {drone.x} 0 {'1' if light else '0'}")
    
    def move_to(self, x, y, light=False):
        print(f"MOVE {x} {y} {'1' if light else '0'}")

    def move_to_radar(self, radar, light=False):
        x = 0 if radar[1] == "L" else 9999
        y = 0 if radar[0] == "T" else 9999

        print(f"MOVE {x} {y} {'1' if light else '0'}")

if __name__ == "__main__":
    bot = Bot()
    bot.play()
    
