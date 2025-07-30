def main():
    # init 
    # initialization input
    my_id = int(input())
    agent_count = int(input())

    my_agent_ids = []

    for _ in range(agent_count):
        (
            agent_id,
            player_id,
            shoot_cooldown,
            optimal_range,
            soaking_power,
            splash_bombs,
        ) = map(int, input().split())

        if player_id == my_id:
            my_agent_ids.append(agent_id)

    agent_one_id = my_agent_ids[0]
    agent_two_id = my_agent_ids[1]

    width, height = map(int, input().split())

    for _ in range(height):
        line = input().split()

    # game loop
    while True:
        # turn input
        remaining_agent_count = int(input())
        for _ in range(remaining_agent_count):
            (
                agent_id,
                x, y,
                cooldown,
                splash_bomb_count,
                wetness,
            ) = map(int, input().split())

        my_remaining_agent_count = int(input())

        print(f"{agent_one_id}; MOVE 6 1")
        print(f"{agent_two_id}; MOVE 6 3")

if __name__ == "__main__":
    main()
