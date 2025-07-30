def main():
    # init 
    # initialization input
    my_id = int(input())
    agent_count = int(input())

    my_agent_ids = []
    ennemy_agent_ids = []

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
        else:
            ennemy_agent_ids.append(agent_id)

    width, height = map(int, input().split())

    for _ in range(height):
        line = input().split()

    # game loop
    while True:
        agent_id_to_wetness = {}

        # turn input
        remaining_agent_count = int(input())
        remaining_ennemy_agent_ids = []
        for _ in range(remaining_agent_count):
            (
                agent_id,
                x, y,
                cooldown,
                splash_bomb_count,
                wetness,
            ) = map(int, input().split())

            agent_id_to_wetness[agent_id] = wetness

            if agent_id in ennemy_agent_ids:
                remaining_ennemy_agent_ids.append(agent_id)

        ennemy_agent_ids = remaining_ennemy_agent_ids

        my_remaining_agent_count = int(input())

        ennemy_agent_ids.sort(
            key=agent_id_to_wetness.get,
            reverse=True
        )

        for my_agent_id in my_agent_ids:
            print(f"{my_agent_id}; SHOOT {ennemy_agent_ids[0]}")

if __name__ == "__main__":
    main()
