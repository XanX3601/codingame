use crate::action;
use crate::bitboard;
use crate::snake;
use crate::zobrist;

#[derive(Debug, PartialEq)]
pub struct GameDefinition {
    enemy_snake_ids: std::collections::HashSet<u8>,
    my_id: u8,
    my_snake_ids: std::collections::HashSet<u8>,
    grid_height: u16,
    grid_width: u16,
    platform_bitboard: bitboard::Bitboard,
}

impl GameDefinition {
    pub const MAX_SNAKE_COUNT: usize = 8;

    pub fn from_buffer<R: std::io::BufRead>(mut reader: R) -> GameDefinition {
        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let my_id = input_line
            .trim()
            .parse::<u8>()
            .unwrap();

        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let grid_width = input_line
            .trim()
            .parse::<u16>()
            .unwrap();

        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let grid_height = input_line
            .trim()
            .parse::<u16>()
            .unwrap();

        let mut platform_bitboard = bitboard::Bitboard::new();
        for y in 0..grid_height {
            let mut input_line = String::new();
            reader.read_line(&mut input_line).unwrap();

            input_line
                .trim()
                .chars()
                .enumerate()
                .for_each(|(x, c)| {
                    match c {
                        '.' => (),
                        '#' => {
                            platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(x as u16, y, grid_width));
                        },
                        _ => panic!("grid should only contain '.' and '#'")
                    }
                })
        }

        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let snakebot_per_player_count = input_line
            .trim()
            .parse::<u32>()
            .unwrap();

        let mut my_snake_ids = std::collections::HashSet::new();
        for _ in 0..snakebot_per_player_count {
            let mut input_line = String::new();
            reader.read_line(&mut input_line).unwrap();
            let my_snake_id = input_line
                .trim()
                .parse::<u8>()
                .unwrap();

            my_snake_ids.insert(my_snake_id);
        }

        let mut enemy_snake_ids = std::collections::HashSet::new();
        for _ in 0..snakebot_per_player_count {
            let mut input_line = String::new();
            reader.read_line(&mut input_line).unwrap();
            let enemy_snake_id = input_line
                .trim()
                .parse::<u8>()
                .unwrap();

            enemy_snake_ids.insert(enemy_snake_id);
        }

        GameDefinition {
            enemy_snake_ids: enemy_snake_ids,
            my_id: my_id,
            my_snake_ids: my_snake_ids,
            grid_height: grid_height,
            grid_width: grid_width,
            platform_bitboard: platform_bitboard,
        }
    }

    pub fn get_grid_height(&self) -> u16 {
        self.grid_height
    }

    pub fn get_grid_width(&self) -> u16 {
        self.grid_width
    }

    pub fn get_my_snake_ids(&self) -> &std::collections::HashSet<u8> {
        &self.my_snake_ids
    }

    pub fn get_platform_bitboard(&self) -> bitboard::Bitboard {
        self.platform_bitboard
    }

    pub fn get_snake_count_per_player(&self) -> usize {
        self.my_snake_ids.len()
    }
}

pub struct GameSimulator {
    collision_bitboard: bitboard::Bitboard,
    gravity_bitboard: bitboard::Bitboard,
    power_sources_to_remove: Vec<usize>,
}

impl GameSimulator {
    pub fn apply(
        &mut self,
        game_state: &GameState,
        actions: [Option<action::Direction>; GameDefinition::MAX_SNAKE_COUNT],
        game_definition: &GameDefinition,
        zobrist_table: &zobrist::ZobristTable,
    ) -> GameState {
        let mut next_game_state = game_state.clone();
        next_game_state.zobrist_hash = 0;

        // collision = platform + other snakes
        self.collision_bitboard = game_definition.platform_bitboard;
        // gravity = platform + other snakes + power sources
        self.gravity_bitboard = game_definition.platform_bitboard.or(
            &game_state.power_source_bitboard
        );

        // first move all snakes and update collision board for later purpose
        for snake_id in 0..GameDefinition::MAX_SNAKE_COUNT {
            if let Some(snake) = next_game_state.snakes[snake_id].as_mut() {

                match actions[snake_id] {
                    Some(direction) => snake.move_toward(direction, game_definition.grid_width, game_definition.grid_height, &zobrist_table, snake_id as u8),
                    _ => snake.move_same_direction(game_definition.grid_width, game_definition.grid_height, &zobrist_table, snake_id as u8),
                }

                // add the entire snake as collision, head not included
                snake.with_headless_body_bitboard(
                    game_definition.grid_width,
                    game_definition.grid_height,
                    |body_bitboar| {self.collision_bitboard.or_inplace(body_bitboar)}
                );

                // add the entire snake as gravity, head included
                self.gravity_bitboard.or_inplace(snake.get_body_bitboard());
                }
        }

        self.power_sources_to_remove.clear();
        
        // second, check if snakes grow, collision with something and then
        // should fall
        for snake_id in 0..GameDefinition::MAX_SNAKE_COUNT {
            if let  Some(snake) = next_game_state.snakes[snake_id].as_mut() {
                if !snake.get_body_bitboard().is_not_empty() {
                    next_game_state.snakes[snake_id] = None;
                    continue;
                }

                let (head_x, head_y) = snake.get_head();

                // check if the snake eats a power source by checking if the head
                // collides with any power sources
                if snake.does_head_collide(
                    &next_game_state.power_source_bitboard,
                    game_definition.grid_width,
                    game_definition.grid_height
                ) {
                    // as snake can eat the same power source, do not remove it right
                    // away, keep it for later
                    // if it collides with the power source bitboard, it is obviously
                    // positive
                    self.power_sources_to_remove.push(
                        bitboard::Bitboard::coord_to_index(head_x as u16, head_y as u16, game_definition.grid_width)
                    );

                    // grow the snake and add its new body part to collision and
                    // gravity
                    snake.grow(game_definition.grid_width, game_definition.grid_height, &zobrist_table, snake_id as u8);
                    snake.with_headless_body_bitboard(
                        game_definition.grid_width,
                        game_definition.grid_height,
                        |body_bitboard| {self.collision_bitboard.or_inplace(body_bitboard)}
                    );
                    self.gravity_bitboard.or_inplace(snake.get_body_bitboard());
                }

                // check if the head collide with the collision bitboard 
                // approximation: two head cannot collide one with another
                if snake.does_head_collide(
                    &self.collision_bitboard,
                    game_definition.grid_width,
                    game_definition.grid_height
                ) {
                    snake.remove_head(game_definition.grid_width, game_definition.grid_height, &zobrist_table, snake_id as u8);
                    // if snake is too small, kill it, it may not be perfect
                    if snake.len() < 3 {
                        next_game_state.snakes[snake_id] = None;
                        continue;
                    }
                }

                // remove the snake body from the gravity so it does not collide
                // with itself
                self.gravity_bitboard.xor_inplace(snake.get_body_bitboard());

                // move the gravity up so it represents every cell above a platform,
                // power source or other snake
                let gravity_up_bitboard = self.gravity_bitboard.move_up(game_definition.grid_width);

                // while snake do not collide with gravity_bitboard, move it down
                let mut step = 0;
                while !snake.does_collide(&gravity_up_bitboard) {
                    snake.move_hitbox_down(game_definition.grid_width);
                    step += 1;
                }

                // once we know how much the snake has to go down, move its body down
                if step > 0 {
                    snake.move_body_down(step, game_definition.grid_width, game_definition.grid_height, &zobrist_table, snake_id as u8);
                }

                // add the snake body back to gravity
                self.gravity_bitboard.or_inplace(snake.get_body_bitboard());

                next_game_state.zobrist_hash ^= snake.get_zobrist_hash();
            }
        }

        // so now, snakes have moved, grew if colliding with power sources
        // shrinked if colliding with anything but head and fall down

        // let's remove the power sources
        self.power_sources_to_remove.iter()
            .for_each(
                |&bit_index| {
                    next_game_state.power_source_bitboard.turn_off(bit_index);
                    next_game_state.power_source_zobrist_hash ^= zobrist_table.get_pwoer_source_hash(bit_index);
                }
            );

        next_game_state.zobrist_hash ^= next_game_state.power_source_zobrist_hash;

        next_game_state
    }

    pub fn new() -> Self {
        Self {
            collision_bitboard: bitboard::Bitboard::new(),
            gravity_bitboard: bitboard::Bitboard::new(),
            power_sources_to_remove: Vec::with_capacity(16),
        }
    }
}

#[derive(Clone, Copy, Debug, PartialEq)]
pub struct GameState {
    power_source_bitboard: bitboard::Bitboard,
    power_source_zobrist_hash: u64,
    snakes: [Option<snake::Snake>; GameDefinition::MAX_SNAKE_COUNT],
    zobrist_hash: u64,
}

impl GameState {
    pub fn evaluate_old(&self, game_definition: &GameDefinition, bitboard_masks: &bitboard::BitboardMasks) -> [f32; GameDefinition::MAX_SNAKE_COUNT] {
        // goal is to compute two score: one for me and one for opponent

        let mut my_frontier = bitboard::Bitboard::new();
        let mut my_obstacles = game_definition.platform_bitboard.clone();
        let mut my_snakes_len = 0f32;
        let mut my_territory_size = 0f32;
        let mut my_power_source_count = 0f32;
        let mut enemy_frontier = bitboard::Bitboard::new();
        let mut enemy_obstacles = game_definition.platform_bitboard.clone();
        let mut enemy_snakes_len = 0f32;
        let mut enemy_territory_size = 0f32;
        let mut enemy_power_source_count = 0f32;

        // prepare the voronoi for my team and enemy
        // also compute the total length 
        for snake_id in 0..GameDefinition::MAX_SNAKE_COUNT {
            if let Some(snake) = self.snakes[snake_id] {
                let is_mine = game_definition.my_snake_ids.contains(&(snake_id as u8));

                let (head_x, head_y) = snake.get_head();
                let is_head_in_grid = snake::Snake::is_in_grid(head_x, head_y, game_definition.grid_width, game_definition.grid_height);

                if is_mine {
                    my_snakes_len += snake.len() as f32;
                    if is_head_in_grid {
                        my_frontier.turn_on(bitboard::Bitboard::coord_to_index(head_x as u16, head_y as u16, game_definition.grid_width));
                        my_territory_size += 1f32;
                        enemy_obstacles.or_inplace(snake.get_body_bitboard());
                    }
                }
                else {
                    enemy_snakes_len += snake.len() as f32;
                    if is_head_in_grid {
                        enemy_frontier.turn_on(bitboard::Bitboard::coord_to_index(head_x as u16, head_y as u16, game_definition.grid_width));
                        enemy_territory_size += 1f32;
                        my_obstacles.or_inplace(snake.get_body_bitboard());
                    }
                }
            }
        }

        // now do the voronoi to compute the territory size
        let mut cells_claimed = true;
        while cells_claimed {
            cells_claimed = false;

            my_obstacles.or_inplace(&my_frontier);
            enemy_obstacles.or_inplace(&enemy_frontier);

            // first extend the enemy frontier and mine
            my_frontier.expand_in_place(game_definition.grid_width, bitboard_masks);
            my_frontier.and_not_inplace(&my_obstacles);
            enemy_frontier.expand_in_place(game_definition.grid_width, bitboard_masks);
            enemy_frontier.and_not_inplace(&enemy_obstacles);

            if my_frontier.is_not_empty() {
                cells_claimed = true;
                enemy_obstacles.or_inplace(&my_frontier);

                my_territory_size += my_frontier.count_ones() as f32;
                my_power_source_count += my_frontier.count_collide(&self.power_source_bitboard) as f32;
            }
            if enemy_frontier.is_not_empty() {
                cells_claimed = true;
                my_obstacles.or_inplace(&enemy_frontier);

                enemy_territory_size += enemy_frontier.count_ones() as f32;
                enemy_power_source_count += enemy_frontier.count_collide(&self.power_source_bitboard) as f32;
            }
        }
        
        let power_source_count: f32 = self.power_source_bitboard.count_ones() as f32;
        let total_grid_area: f32 = (game_definition.grid_width * game_definition.grid_height) as f32;
        let max_posible_len: f32 = my_snakes_len + enemy_snakes_len + power_source_count;

        let my_score = {
            0.3 * my_territory_size / total_grid_area
            + 0.3 * my_snakes_len / max_posible_len
            + 0.4 * my_power_source_count / power_source_count
        };
        let enemy_score = {
            0.3 * enemy_territory_size / total_grid_area
            + 0.3 * enemy_snakes_len / max_posible_len
            + 0.4 * enemy_power_source_count / power_source_count
        };

        let mut scores = [0f32; GameDefinition::MAX_SNAKE_COUNT];

        for snake_id in 0..GameDefinition::MAX_SNAKE_COUNT {
            if let Some(snake) = self.snakes[snake_id] {
                let is_mine = game_definition.my_snake_ids.contains(&(snake_id as u8));

                scores[snake_id] = if is_mine {my_score} else {enemy_score};
            }
        }

        scores
    }

    pub fn evaluate(&self, game_definition: &GameDefinition, bitboard_masks: &bitboard::BitboardMasks) -> [f32; GameDefinition::MAX_SNAKE_COUNT] {


        let mut scores = [0f32; GameDefinition::MAX_SNAKE_COUNT];

        scores
    }

    pub fn get_snake(&self, snake_id: usize) -> &Option<snake::Snake> {
        &self.snakes[snake_id]
    }

    pub fn get_zobrist_hash(&self) -> u64 {
        self.zobrist_hash
    }

    pub fn new() -> Self {
        GameState {
            power_source_bitboard: bitboard::Bitboard::new(),
            power_source_zobrist_hash: 0,
            snakes: [None; GameDefinition::MAX_SNAKE_COUNT],
            zobrist_hash: 0,
        }
    }

    pub fn update_from_buffer<R: std::io::BufRead>(&mut self, mut reader: R, width: u16, height: u16, zobrist_table: &zobrist::ZobristTable) {
        self.power_source_bitboard.clear();
        self.power_source_zobrist_hash = 0;

        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let power_source_count = input_line
            .trim()
            .parse::<u32>()
            .unwrap();

        for _ in 0..power_source_count {
            let mut input_line = String::new();
            reader.read_line(&mut input_line).unwrap();
            let mut power_source_coord_iter = input_line
                .trim()
                .split_whitespace()
                .into_iter();
            let power_source_index = bitboard::Bitboard::coord_to_index(
                power_source_coord_iter.next().unwrap().parse::<u16>().unwrap(), 
                power_source_coord_iter.next().unwrap().parse::<u16>().unwrap(), 
                width
            );
            self.power_source_bitboard.turn_on(power_source_index);
            self.power_source_zobrist_hash ^= zobrist_table.get_pwoer_source_hash(power_source_index);
        }

        self.zobrist_hash = self.power_source_zobrist_hash;

        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let snakebot_count = input_line
            .trim()
            .parse::<u32>()
            .unwrap();

        let mut snake_ids = std::collections::HashSet::new();
        for _ in 0..snakebot_count {
            let mut input_line = String::new();
            reader.read_line(&mut input_line).unwrap();
            let mut snake_iter = input_line
                .trim()
                .split_whitespace()
                .into_iter();
            let snake_id = snake_iter.next().unwrap().parse::<u8>().unwrap();
            snake_ids.insert(snake_id);
            let snake = self.snakes[snake_id as usize].get_or_insert(snake::Snake::new());
            snake.clear();
            snake_iter
                .next()
                .unwrap()
                .split(':')
                .for_each(
                    |body_part_coord| {
                        let mut body_part_coord_iter = body_part_coord.split(',');
                        let x = body_part_coord_iter
                            .next()
                            .unwrap()
                            .parse::<i16>()
                            .unwrap();
                        let y = body_part_coord_iter
                            .next()
                            .unwrap()
                            .parse::<i16>()
                            .unwrap();
                        snake.add_body_part(x, y, width, height, &zobrist_table, snake_id);
                    }
                );

            self.zobrist_hash ^= snake.get_zobrist_hash();
        }

        for snake_id in 0..GameDefinition::MAX_SNAKE_COUNT {
            if !snake_ids.contains(&(snake_id as u8)) {
                self.snakes[snake_id] = None;
            }
        }
    }
}

#[cfg(test)]
mod test {
    use super::*;
    // use pretty_assertions::assert_eq;

    #[test] fn can_apply_actions() {
        let zobrist_table = zobrist::ZobristTable::new();

        let input = r#"0
            18
            10
            ..................
            ..................
            ..................
            ..#............#..
            ...#....##....#...
            ..................
            ..................
            .......#..#.......
            ..#...##..##...#..
            ##################
            2
            0
            1
            2
            3
        "#;

        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);

        let game_definition = GameDefinition::from_buffer(reader);

        let bitboard_masks = bitboard::BitboardMasks::new(
            game_definition.grid_width,
            game_definition.grid_height
        );

        let mut game_state = GameState::new();

        let input = r#"18
            0 2
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            0 1
            17 1
            5 4
            12 4
            0 5
            17 5
            5 6
            12 6
            0 7
            17 7
            4
            0 2,5:2,6:2,7
            1 15,0:15,1:15,2
            2 15,5:15,6:15,7
            3 2,0:2,1:2,2
        "#;

        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);

        game_state.update_from_buffer(
            reader,
            game_definition.grid_width,
            game_definition.grid_height,
            &zobrist_table
        );

        let mut simulator = GameSimulator::new();

        let mut expected_game_state = game_state.clone();

        let mut actions: [Option<action::Direction>; GameDefinition::MAX_SNAKE_COUNT] = [None; GameDefinition::MAX_SNAKE_COUNT];
        actions[1] = Some(action::Direction::Right);
        actions[2] = Some(action::Direction::Right);
        actions[3] = Some(action::Direction::Left);
        let game_state = simulator.apply(&game_state, actions, &game_definition, &zobrist_table);

        let input = r#"18
            0 2
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            0 1
            17 1
            5 4
            12 4
            0 5
            17 5
            5 6
            12 6
            0 7
            17 7
            4
            0 2,5:2,6:2,7
            1 16,1:15,1:15,2
            2 16,6:15,6:15,7
            3 1,1:2,1:2,2
        "#;
        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);
        expected_game_state.update_from_buffer(
            reader,
            game_definition.grid_width,
            game_definition.grid_height,
            &zobrist_table
        );

        assert_eq!(game_state, expected_game_state);

        actions[1] = Some(action::Direction::Right);
        actions[2] = Some(action::Direction::Up);
        actions[3] = Some(action::Direction::Left);
        let game_state = simulator.apply(&game_state, actions, &game_definition, &zobrist_table);

        let input = r#"16
            0 2
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            5 4
            12 4
            0 5
            17 5
            5 6
            12 6
            0 7
            17 7
            4
            0 2,5:2,6:2,7
            1 17,1:16,1:15,1:15,2
            2 16,6:16,7:15,7
            3 0,1:1,1:2,1:2,2
        "#;
        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);
        expected_game_state.update_from_buffer(
            reader,
            game_definition.grid_width,
            game_definition.grid_height,
            &zobrist_table,
        );

        assert_eq!(game_state, expected_game_state);

        actions[1] = Some(action::Direction::Right);
        actions[2] = Some(action::Direction::Up);
        actions[3] = Some(action::Direction::Down);
        let game_state = simulator.apply(&game_state, actions, &game_definition, &zobrist_table);

        let input = r#"15
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            5 4
            12 4
            0 5
            17 5
            5 6
            12 6
            0 7
            17 7
            4
            0 2,5:2,6:2,7
            1 18,1:17,1:16,1:15,1
            2 16,6:16,7:16,8
            3 0,2:0,1:1,1:2,1:2,2
        "#;
        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);
        expected_game_state.update_from_buffer(
            reader,
            game_definition.grid_width,
            game_definition.grid_height,
            &zobrist_table,
        );

        assert_eq!(game_state, expected_game_state);

        actions[1] = Some(action::Direction::Right);
        actions[2] = Some(action::Direction::Up);
        actions[3] = Some(action::Direction::Down);
        let game_state = simulator.apply(&game_state, actions, &game_definition, &zobrist_table);

        let input = r#"15
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            5 4
            12 4
            0 5
            17 5
            5 6
            12 6
            0 7
            17 7
            4
            0 2,5:2,6:2,7
            1 19,1:18,1:17,1:16,1
            2 16,6:16,7:16,8
            3 0,4:0,3:0,2:1,2:2,2
        "#;
        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);
        expected_game_state.update_from_buffer(
            reader,
            game_definition.grid_width,
            game_definition.grid_height,
            &zobrist_table,
        );

        assert_eq!(game_state, expected_game_state);

        actions[1] = Some(action::Direction::Right);
        actions[2] = Some(action::Direction::Up);
        actions[3] = Some(action::Direction::Down);
        let game_state = simulator.apply(&game_state, actions, &game_definition, &zobrist_table);

        let input = r#"14
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            5 4
            12 4
            17 5
            5 6
            12 6
            0 7
            17 7
            4
            0 2,5:2,6:2,7
            1 20,1:19,1:18,1:17,1
            2 16,6:16,7:16,8
            3 0,5:0,4:0,3:0,2:1,2:2,2
        "#;
        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);
        expected_game_state.update_from_buffer(
            reader,
            game_definition.grid_width,
            game_definition.grid_height,
            &zobrist_table,
        );

        assert_eq!(game_state, expected_game_state);

        actions[1] = Some(action::Direction::Right);
        actions[2] = Some(action::Direction::Up);
        actions[3] = Some(action::Direction::Down);
        let game_state = simulator.apply(&game_state, actions, &game_definition, &zobrist_table);

        let input = r#"14
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            5 4
            12 4
            17 5
            5 6
            12 6
            0 7
            17 7
            3
            0 2,5:2,6:2,7
            2 16,6:16,7:16,8
            3 0,6:0,5:0,4:0,3:0,2:1,2
        "#;
        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);
        expected_game_state.update_from_buffer(
            reader,
            game_definition.grid_width,
            game_definition.grid_height,
            &zobrist_table,
        );

        assert_eq!(game_state, expected_game_state);

        actions[1] = Some(action::Direction::Right);
        actions[2] = Some(action::Direction::Up);
        actions[3] = Some(action::Direction::Down);
        let game_state = simulator.apply(&game_state, actions, &game_definition, &zobrist_table);

        let input = r#"13
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            5 4
            12 4
            17 5
            5 6
            12 6
            17 7
            3
            0 2,5:2,6:2,7
            2 16,6:16,7:16,8
            3 0,8:0,7:0,6:0,5:0,4:0,3:1,3
        "#;
        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);
        expected_game_state.update_from_buffer(
            reader,
            game_definition.grid_width,
            game_definition.grid_height,
            &zobrist_table,
        );

        assert_eq!(game_state, expected_game_state);

        actions[1] = Some(action::Direction::Right);
        actions[2] = Some(action::Direction::Up);
        let game_state = simulator.apply(&game_state, actions, &game_definition, &zobrist_table);

        let input = r#"13
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            5 4
            12 4
            17 5
            5 6
            12 6
            17 7
            3
            0 2,5:2,6:2,7
            2 16,6:16,7:16,8
            3 0,8:0,7:0,6:0,5:0,4:0,3
        "#;
        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);
        expected_game_state.update_from_buffer(
            reader,
            game_definition.grid_width,
            game_definition.grid_height,
            &zobrist_table,
        );

        assert_eq!(game_state, expected_game_state);

        actions[1] = Some(action::Direction::Right);
        actions[2] = Some(action::Direction::Up);
        let game_state = simulator.apply(&game_state, actions, &game_definition, &zobrist_table);

        let input = r#"13
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            5 4
            12 4
            17 5
            5 6
            12 6
            17 7
            3
            0 2,5:2,6:2,7
            2 16,6:16,7:16,8
            3 0,8:0,7:0,6:0,5:0,4
        "#;
        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);
        expected_game_state.update_from_buffer(
            reader,
            game_definition.grid_width,
            game_definition.grid_height,
            &zobrist_table,
        );

        assert_eq!(game_state, expected_game_state);

        actions[1] = Some(action::Direction::Right);
        actions[2] = Some(action::Direction::Up);
        let game_state = simulator.apply(&game_state, actions, &game_definition, &zobrist_table);

        let input = r#"13
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            5 4
            12 4
            17 5
            5 6
            12 6
            17 7
            3
            0 2,5:2,6:2,7
            2 16,6:16,7:16,8
            3 0,8:0,7:0,6:0,5
        "#;
        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);
        expected_game_state.update_from_buffer(
            reader,
            game_definition.grid_width,
            game_definition.grid_height,
            &zobrist_table,
        );

        assert_eq!(game_state, expected_game_state);

        actions[1] = Some(action::Direction::Right);
        actions[2] = Some(action::Direction::Up);
        let game_state = simulator.apply(&game_state, actions, &game_definition, &zobrist_table);

        let input = r#"13
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            5 4
            12 4
            17 5
            5 6
            12 6
            17 7
            3
            0 2,5:2,6:2,7
            2 16,6:16,7:16,8
            3 0,8:0,7:0,6
        "#;
        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);
        expected_game_state.update_from_buffer(
            reader,
            game_definition.grid_width,
            game_definition.grid_height,
            &zobrist_table,
        );

        assert_eq!(game_state, expected_game_state);

        actions[1] = Some(action::Direction::Right);
        actions[2] = Some(action::Direction::Up);
        let game_state = simulator.apply(&game_state, actions, &game_definition, &zobrist_table);

        let input = r#"13
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            5 4
            12 4
            17 5
            5 6
            12 6
            17 7
            2
            0 2,5:2,6:2,7
            2 16,6:16,7:16,8
        "#;
        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);
        expected_game_state.update_from_buffer(
            reader,
            game_definition.grid_width,
            game_definition.grid_height,
            &zobrist_table,
        );

        assert_eq!(game_state, expected_game_state);

        actions[1] = Some(action::Direction::Right);
        actions[2] = Some(action::Direction::Up);
        let game_state = simulator.apply(&game_state, actions, &game_definition, &zobrist_table);

        let input = r#"13
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            5 4
            12 4
            17 5
            5 6
            12 6
            17 7
            2
            0 2,5:2,6:2,7
            2 16,6:16,7:16,8
        "#;
        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);
        expected_game_state.update_from_buffer(
            reader,
            game_definition.grid_width,
            game_definition.grid_height,
            &zobrist_table,
        );

        assert_eq!(game_state, expected_game_state);
    }

    #[test]
    fn can_create_game_definition_from_buffer() {
        let input = r#"0
            26
            14
            ..........................
            ..........................
            .........#......#.........
            ........#........#........
            ....##..............##....
            .......#..........#.......
            ........##......##........
            ..#....................#..
            ...#..................#...
            ##.##.......##.......##.##
            .....##....####....##.....
            .....##...######...##.....
            .#..###..########..###..#.
            ##########################
            3
            0
            1
            2
            3
            4
            5
        "#;

        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);

        let game_definition = GameDefinition::from_buffer(reader);

        let mut expected_enemy_snake_ids = std::collections::HashSet::new();
        expected_enemy_snake_ids.insert(3);
        expected_enemy_snake_ids.insert(4);
        expected_enemy_snake_ids.insert(5);
        assert_eq!(game_definition.enemy_snake_ids, expected_enemy_snake_ids);
        assert_eq!(game_definition.my_id, 0);
        let mut expected_my_snake_ids = std::collections::HashSet::new();
        expected_my_snake_ids.insert(0);
        expected_my_snake_ids.insert(1);
        expected_my_snake_ids.insert(2);
        assert_eq!(game_definition.my_snake_ids, expected_my_snake_ids);
        assert_eq!(game_definition.grid_height, 14);
        assert_eq!(game_definition.grid_width, 26);
        let width = 26;
        let mut expected_platform_bitboard = bitboard::Bitboard::new();
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(9, 2, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(16, 2, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(8, 3, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(17, 3, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(4, 4, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(5, 4, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(20, 4, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(21, 4, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(7, 5, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(18, 5, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(8, 6, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(9, 6, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(16, 6, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(17, 6, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(2, 7, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(23, 7, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(3, 8, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(22, 8, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(0, 9, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 9, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(3, 9, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(4, 9, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(12, 9, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(13, 9, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(21, 9, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(22, 9, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(24, 9, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(25, 9, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(5, 10, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(6, 10, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(11, 10, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(12, 10, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(13, 10, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(14, 10, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(19, 10, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(20, 10, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(5, 11, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(6, 11, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(10, 11, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(11, 11, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(12, 11, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(13, 11, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(14, 11, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(15, 11, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(19, 11, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(20, 11, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 12, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(4, 12, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(5, 12, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(6, 12, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(9, 12, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(10, 12, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(11, 12, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(12, 12, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(13, 12, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(14, 12, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(15, 12, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(16, 12, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(19, 12, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(20, 12, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(21, 12, width));
        expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(24, 12, width));
        for x in 0..26 {
            expected_platform_bitboard.turn_on(bitboard::Bitboard::coord_to_index(x, 13, width));
        }
        assert_eq!(game_definition.platform_bitboard, expected_platform_bitboard);
    }

    #[test]
    fn can_create_game_state() {
        let game_state = GameState::new();

        let expected_power_source_bitboard = bitboard::Bitboard::new();
        assert_eq!(game_state.power_source_bitboard, expected_power_source_bitboard);
        assert_eq!(game_state.snakes, [None; GameDefinition::MAX_SNAKE_COUNT]);
    }

    #[test] fn can_evaluate() {
        let zobrist_table = zobrist::ZobristTable::new();

        let input = r#"0
            18
            10
            ..................
            ..................
            ..................
            ..#............#..
            ...#....##....#...
            ..................
            ..................
            .......#..#.......
            ..#...##..##...#..
            ##################
            2
            0
            1
            2
            3
        "#;

        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);

        let game_definition = GameDefinition::from_buffer(reader);

        let bitboard_masks = bitboard::BitboardMasks::new(
            game_definition.grid_width,
            game_definition.grid_height
        );

        let mut game_state = GameState::new();

        let input = r#"18
            0 2
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            0 1
            17 1
            5 4
            12 4
            0 5
            17 5
            5 6
            12 6
            0 7
            17 7
            4
            0 2,5:2,6:2,7
            1 15,0:15,1:15,2
            2 15,5:15,6:15,7
            3 2,0:2,1:2,2
        "#;

        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);

        game_state.update_from_buffer(
            reader,
            game_definition.grid_width,
            game_definition.grid_height,
            &zobrist_table
        );

        let evaluation = game_state.evaluate(&game_definition, &bitboard_masks);
    }

    #[test]
    fn can_update_game_state_from_buffer() {
        let zobrist_table = zobrist::ZobristTable::new();

        let width = 26;
        let height = 14;

        let mut game_state = GameState::new();

        let input = r#"5
            0 0
            24 10
            1 1
            13 3
            8 10
            4
            4 0,0:1,0:1,1
            1 20,10:19,10:19,9:19,8
            3 10,10:11,10:12,10:13,10
            0 5,5:6,5:7,5:8,5
        "#;

        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);

        game_state.update_from_buffer(reader, width, height, &zobrist_table);

        let mut expected_power_source_bitboard = bitboard::Bitboard::new();
        expected_power_source_bitboard.turn_on(bitboard::Bitboard::coord_to_index(0, 0, width));
        expected_power_source_bitboard.turn_on(bitboard::Bitboard::coord_to_index(24, 10, width));
        expected_power_source_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 1, width));
        expected_power_source_bitboard.turn_on(bitboard::Bitboard::coord_to_index(13, 3, width));
        expected_power_source_bitboard.turn_on(bitboard::Bitboard::coord_to_index(8, 10, width));
        assert_eq!(game_state.power_source_bitboard, expected_power_source_bitboard);

        let mut expected_snakes = [None; GameDefinition::MAX_SNAKE_COUNT];
        let mut snake = snake::Snake::new();
        snake.add_body_part(0, 0, width, height, &zobrist_table, 4);
        snake.add_body_part(1, 0, width, height, &zobrist_table, 4);
        snake.add_body_part(1, 1, width, height, &zobrist_table, 4);
        expected_snakes[4] = Some(snake);

        let mut snake = snake::Snake::new();
        snake.add_body_part(20, 10, width, height, &zobrist_table, 1);
        snake.add_body_part(19, 10, width, height, &zobrist_table, 1);
        snake.add_body_part(19, 9, width, height, &zobrist_table, 1);
        snake.add_body_part(19, 8, width, height, &zobrist_table, 1);
        expected_snakes[1] = Some(snake);

        let mut snake = snake::Snake::new();
        snake.add_body_part(10, 10, width, height, &zobrist_table, 3);
        snake.add_body_part(11, 10, width, height, &zobrist_table, 3);
        snake.add_body_part(12, 10, width, height, &zobrist_table, 3);
        snake.add_body_part(13, 10, width, height, &zobrist_table, 3);
        expected_snakes[3] = Some(snake);

        let mut snake = snake::Snake::new();
        snake.add_body_part(5, 5, width, height, &zobrist_table, 0);
        snake.add_body_part(6, 5, width, height, &zobrist_table, 0);
        snake.add_body_part(7, 5, width, height, &zobrist_table, 0);
        snake.add_body_part(8, 5, width, height, &zobrist_table, 0);
        expected_snakes[0] = Some(snake);

        assert_eq!(game_state.snakes, expected_snakes);
    }
}
