import sys

def log(*args):
    print(*args, file=sys.stderr)

player_count = 3

class HurdleRaceBot:
    def __init__(self, my_player_index, debug=False):
        self.my_player_index = my_player_index
        self.debug = debug

    def _load_registers_from_input(self):
        registers = input().split()
        self.map = registers[0]
        self.my_position_index = int(registers[self.my_player_index + 1])

        if self.debug:
            self._log(self.map)
            self._log("my position index:", self.my_position_index)

    def _log(self, *args):
        log("hurdle race bot:", *args)

    def play(self):
        self._load_registers_from_input()

        try:
            distance_to_next_hurdle = self.map[self.my_position_index:].index("#")
            
            if self.debug:
                self._log("distance to next hurdle", distance_to_next_hurdle)

            if distance_to_next_hurdle == 1:
                print("UP")
            elif distance_to_next_hurdle == 2:
                print("LEFT")
            elif distance_to_next_hurdle == 3:
                print("DOWN")
            else:
                print("RIGHT")
        except ValueError: # no more hurdle
            print("RIGHT")

if __name__ == "__main__":
    my_player_index = int(input())
    game_count = int(input())

    bot = HurdleRaceBot(my_player_index)

    while True:
        for player_id in range(player_count):
            input() # score info, not used for now

        for game_id in range(game_count):
            bot.play()
