import random
import sys

my_player_index = int(input())
mini_game_count = int(input())

player_count = 3
register_count = 7


def log(*args):
    print(*args, file=sys.stderr)

class ScoreBoard:
    def __init__(self):
        self.player_final_scores = [0 for _ in range(player_count)]
        self.player_medals_per_game = [
            [[0, 0, 0] for __ in range(mini_game_count)]
            for _ in range(player_count)
        ]

    def update(self):
        for player_index in range(player_count):
            score_info = input().split()

            self.player_final_scores[player_index] = int(score_info[0])

            score_info = score_info[1:]

            for game_index in range(mini_game_count):
                medals = self.player_medals_per_game[player_index][game_index]
                medals[0] = int(score_info[0])
                medals[1] = int(score_info[1])
                medals[2] = int(score_info[2])
                score_info = score_info[3:]

class MiniGame:
    def __init__(self):
        self.gpu = "GAME OVER"
        self.registers = [0 for _ in range(register_count)]

    def is_resetting(self):
        return self.gpu == "GAME OVER"

    def update(self):
        data = input().split()
        self.gpu = data[0]
        for register_index in range(register_count):
            self.registers[register_index] = int(data[register_index + 1])

class HurdleRace(MiniGame):
    def __init__(self):
        super().__init__()

    def am_i_stunned(self):
        return self.registers[player_count + my_player_index] > 0

    def next_hurdle_position(self, position_index):
        if self.is_resetting():
            return None

        gpu = self.gpu[position_index:]
        next_hurdle_position = gpu.find("#")

        if next_hurdle_position >= 0:
            next_hurdle_position += position_index

        return next_hurdle_position

    def get_my_position_index(self):
        return self.registers[my_player_index]

class HurdleRaceBot:
    def __init__(self, hurdle_race):
        self.hurdle_race = hurdle_race

    def compute_best_action(self):
        if (
            self.hurdle_race.is_resetting()
            or self.hurdle_race.am_i_stunned()
        ):
            return None

        my_position_index = self.hurdle_race.get_my_position_index()
        next_hurdle_position = self.hurdle_race.next_hurdle_position(my_position_index)

        if next_hurdle_position < 0:
            return "RIGHT"

        distance_to_next_hurdle = next_hurdle_position - my_position_index

        if distance_to_next_hurdle == 1:
            return "UP"
        elif distance_to_next_hurdle == 2:
            return "LEFT"
        elif distance_to_next_hurdle == 3:
            return "DOWN"
        else:
            return "RIGHT"

if __name__ == "__main__":
    score_board = ScoreBoard()
    mini_games = [HurdleRace() for _ in range(mini_game_count)]
    bots = [HurdleRaceBot(hurdle_race) for hurdle_race in mini_games]

    while True:
        score_board.update()
        for mini_game in mini_games:
            mini_game.update()

        action_to_weight = {}

        for bot in bots:
            best_action = bot.compute_best_action()

            if best_action is None:
                continue

            if best_action not in action_to_weight:
                action_to_weight[best_action] = 1
            else:
                action_to_weight[best_action] += 1

        if not action_to_weight:
            print("UP")
            continue

        action = max(action_to_weight.keys(), key=action_to_weight.get)

        print(action)


