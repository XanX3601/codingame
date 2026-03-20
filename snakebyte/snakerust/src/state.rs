use crate::action;
use crate::bitboard;
use crate::snake;

#[derive(Debug, PartialEq)]
pub struct GameDefinition {
    enemy_snake_ids: std::collections::HashSet<u32>,
    my_id: u32,
    my_snake_ids: std::collections::HashSet<u32>,
    gravity_bitboard: bitboard::Bitboard,
    grid_height: u32,
    grid_width: u32,
    platform_bitboard: bitboard::Bitboard,
}

impl GameDefinition {
    pub fn from_buffer<R: std::io::BufRead>(mut reader: R) -> GameDefinition {
        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let my_id = input_line
            .trim()
            .parse::<u32>()
            .unwrap();

        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let grid_width = input_line
            .trim()
            .parse::<u32>()
            .unwrap();

        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let grid_height = input_line
            .trim()
            .parse::<u32>()
            .unwrap();

        let mut platform_bitboard = bitboard::Bitboard::new(grid_width);
        let mut gravity_bitboard = bitboard::Bitboard::new(grid_width);
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
                            platform_bitboard.turn_on(x as u32, y);
                            if y - 1 >= 0 {gravity_bitboard.turn_on(x as u32, y - 1);};
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
                .parse::<u32>()
                .unwrap();

            my_snake_ids.insert(my_snake_id);
        }

        let mut enemy_snake_ids = std::collections::HashSet::new();
        for _ in 0..snakebot_per_player_count {
            let mut input_line = String::new();
            reader.read_line(&mut input_line).unwrap();
            let enemy_snake_id = input_line
                .trim()
                .parse::<u32>()
                .unwrap();

            enemy_snake_ids.insert(enemy_snake_id);
        }

        GameDefinition {
            enemy_snake_ids: enemy_snake_ids,
            my_id: my_id,
            my_snake_ids: my_snake_ids,
            gravity_bitboard: gravity_bitboard,
            grid_height: grid_height,
            grid_width: grid_width,
            platform_bitboard: platform_bitboard,
        }
    }

    pub fn get_snake_count_per_player(&self) -> usize {
        self.my_snake_ids.len()
    }
}

pub struct GameSimulator {
    collision_bitboard: bitboard::Bitboard,
    gravity_bitboard: bitboard::Bitboard,
    power_sources_to_remove: Vec<(u32, u32)>,
}

impl<'a> GameSimulator {
    pub fn apply(&mut self, game_state: &GameState<'a>, actions: &std::collections::HashMap<u32, action::Direction>) -> GameState<'a> {
        let mut next_game_state = game_state.clone();

        // collision = platform + other snakes
        self.collision_bitboard = game_state.game_definition.platform_bitboard.clone();
        // gravity = platform + other snakes + power sources
        self.gravity_bitboard = game_state.game_definition.platform_bitboard.or(
            &game_state.power_source_bitboard
        );

        // first move all snakes and update collision board for later purpose
        for (snake_id, snake) in next_game_state.snake_id_to_snake.iter_mut() {
            if actions.contains_key(snake_id) {
                snake.move_toward(actions[snake_id].clone());
            }
            else {
                snake.move_same_direction();
            }

            if *snake_id == 1 {
                println!("{snake:?}");
                let is_not_empty = snake.get_body_bitboard().is_not_empty();
                println!("{is_not_empty:?}")
            }

            // add the entire snake as collision, head not included
            snake.with_headless_body_bitboard(
                |body_bitboar| {self.collision_bitboard.or_inplace(body_bitboar)}
            );

            // add the entire snake as gravity, head included
            self.gravity_bitboard.or_inplace(snake.get_body_bitboard());
            if *snake_id == 1 {
                println!("gravity bitboard after snake 1 moved");
                for y in 0..10 {
                    for x in 0..18 {
                        let c = if self.gravity_bitboard.is_on(x, y) {'#'} else {'.'};
                        print!("{c} ");
                    }
                    println!("");
                }
                println!("");
            }
        }

        self.power_sources_to_remove.clear();
        
        // second, check if snakes grow, collision with something and then
        // should fall
        for (snake_id, snake) in next_game_state.snake_id_to_snake.iter_mut() {
            if !snake.get_body_bitboard().is_not_empty() {
                continue;
            }

            let (head_x, head_y) = snake.get_head();

            if *snake_id == 1 {
                println!("snake 1 head before colliding with power source");
                for y in 0..10 {
                    for x in 0..18 {
                        let c = if snake.get_body_bitboard().is_on(x, y) {'#'} else {'.'};
                        print!("{c} ");
                    }
                    println!("");
                }
                let head = snake.get_head();
                println!("head {head:?}");
                println!("");
            }

            if *snake_id == 1 {
                println!("power sources bitboard before snake 1 grows");
                for y in 0..10 {
                    for x in 0..18 {
                        let c = if next_game_state.power_source_bitboard.is_on(x, y) {'#'} else {'.'};
                        print!("{c} ");
                    }
                    println!("");
                }
                println!("");
            }

            // check if the snake eats a power source by checking if the head
            // collides with any power sources
            if snake.does_head_collide(&next_game_state.power_source_bitboard) {
                // as snake can eat the same power source, do not remove it right
                // away, keep it for later
                // if it collides with the power source bitboard, it is obviously
                // positive
                self.power_sources_to_remove.push((*head_x as u32, *head_y as u32));

                // grow the snake and add its new body part to collision and
                // gravity
                snake.grow();
                snake.with_headless_body_bitboard(
                    |body_bitboar| {self.collision_bitboard.or_inplace(body_bitboar)}
                );
                self.gravity_bitboard.or_inplace(snake.get_body_bitboard());

                if *snake_id == 1 {
                    println!("gravity bitboard after snake 1 grew");
                    for y in 0..10 {
                        for x in 0..18 {
                            let c = if self.gravity_bitboard.is_on(x, y) {'#'} else {'.'};
                            print!("{c} ");
                        }
                        println!("");
                    }
                    println!("");
                }
            }

            // check if the head collide with the collision bitboard 
            // approximation: two head cannot collide one with another
            if snake.does_head_collide(&self.collision_bitboard) {
                snake.remove_head();
            }

            if *snake_id == 1 {
                println!("gravity bitboard before substracting snake bitboard");
                for y in 0..10 {
                    for x in 0..18 {
                        let c = if self.gravity_bitboard.is_on(x, y) {'#'} else {'.'};
                        print!("{c} ");
                    }
                    println!("");
                }
                println!("");
            }

            if *snake_id == 1 {
                println!("snake 1 bitboard before removing it from gravity");
                for y in 0..10 {
                    for x in 0..18 {
                        let c = if snake.get_body_bitboard().is_on(x, y) {'#'} else {'.'};
                        print!("{c} ");
                    }
                    println!("");
                }
                println!("");
            }

            // remove the snake body from the gravity so it does not collide
            // with itself
            self.gravity_bitboard.xor_inplace(snake.get_body_bitboard());

            if *snake_id == 1 {
                println!("gravity bitboard after snake is removed from it");
                for y in 0..10 {
                    for x in 0..18 {
                        let c = if self.gravity_bitboard.is_on(x, y) {'#'} else {'.'};
                        print!("{c} ");
                    }
                    println!("");
                }
                println!("");
            }


            // move the gravity up so it represents every cell above a platform,
            // power source or other snake
            let gravity_up_bitboard = self.gravity_bitboard.move_up();

            // while snake do not collide with gravity_bitboard, move it down
            let mut step = 0;
            while !snake.does_collide(&gravity_up_bitboard) {
                snake.move_hitbox_down();
                step += 1;
            }

            // once we know how much the snake has to go down, move its body down
            if step > 0 {
                snake.move_body_down(step);
            }

            // add the snake body back to gravity
            self.gravity_bitboard.or_inplace(snake.get_body_bitboard());
        }

        // so now, snakes have moved, grew if colliding with power sources
        // shrinked if colliding with anything but head and fall down

        // let's remove the power sources
        for (x, y) in self.power_sources_to_remove.iter() {
            next_game_state.power_source_bitboard.turn_off(*x, *y);
        }

        // remove the snakes with len < 3, they are dead
        next_game_state.snake_id_to_snake
            .retain(|&snake_id, snake| {
                let len = snake.len();
                let is_not_empty = snake.get_body_bitboard().is_not_empty();
                let retain = len >= 3 && is_not_empty;
                println!("retain ? {len:?} {is_not_empty:?} ? {retain:?}");

                    retain
            });

        println!("after death: {:?}", next_game_state.snake_id_to_snake);

        next_game_state
    }

    pub fn new(grid_width: u32) -> GameSimulator {
        GameSimulator{
            collision_bitboard: bitboard::Bitboard::new(grid_width),
            gravity_bitboard: bitboard::Bitboard::new(grid_width),
            power_sources_to_remove: Vec::with_capacity(4),
        }
    }
}

#[derive(Clone, Debug, PartialEq)]
pub struct GameState<'a> {
    game_definition: &'a GameDefinition,
    power_source_bitboard: bitboard::Bitboard,
    snake_id_to_snake: std::collections::HashMap<u32, snake::Snake>,
}

impl<'a> GameState<'a> {
    pub fn get_game_definition(&self) -> &GameDefinition {
        self.game_definition
    }

    pub fn new(game_definition: &'a GameDefinition) -> GameState<'a> {
        let mut snake_id_to_snake: std::collections::HashMap<u32, snake::Snake> = std::collections::HashMap::new();
        for snake_id in &game_definition.enemy_snake_ids {
            snake_id_to_snake.insert(*snake_id, snake::Snake::new(game_definition.grid_width));
        }
        for snake_id in &game_definition.my_snake_ids {
            snake_id_to_snake.insert(*snake_id, snake::Snake::new(game_definition.grid_width));
        }

        GameState {
            game_definition: game_definition,
            power_source_bitboard: bitboard::Bitboard::new(game_definition.grid_width),
            snake_id_to_snake: snake_id_to_snake,
        }
    }

    pub fn update_from_buffer<R: std::io::BufRead>(&mut self, mut reader: R) {
        self.power_source_bitboard.clear();

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
            self.power_source_bitboard.turn_on(
                power_source_coord_iter.next().unwrap().parse::<u32>().unwrap(),
                power_source_coord_iter.next().unwrap().parse::<u32>().unwrap()
            );
        }

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
            let snake_id = snake_iter.next().unwrap().parse::<u32>().unwrap();
            snake_ids.insert(snake_id);
            let snake = self.snake_id_to_snake.get_mut(&snake_id).unwrap();
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
                            .parse::<i32>()
                            .unwrap();
                        let y = body_part_coord_iter
                            .next()
                            .unwrap()
                            .parse::<i32>()
                            .unwrap();
                        snake.add_body_part(x, y);
                    }
                )
        }

        self.snake_id_to_snake.retain(|snake_id, _| snake_ids.contains(snake_id));
    }
}

#[cfg(test)]
mod test {
    use super::*;
    use pretty_assertions::assert_eq;

    #[test] fn can_apply_actions() {
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

        let mut game_state = GameState::new(&game_definition);

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

        game_state.update_from_buffer(reader);

        let mut simulator = GameSimulator::new(game_definition.grid_width);

        let mut expected_game_state = game_state.clone();

        let mut actions = std::collections::HashMap::new();
        actions.insert(1, action::Direction::Right);
        actions.insert(2, action::Direction::Right);
        actions.insert(3, action::Direction::Left);
        let game_state = simulator.apply(&game_state, &actions);

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
        expected_game_state.update_from_buffer(reader);

        assert_eq!(game_state, expected_game_state);

        let mut actions = std::collections::HashMap::new();
        actions.insert(1, action::Direction::Right);
        actions.insert(2, action::Direction::Up);
        actions.insert(3, action::Direction::Left);
        let game_state = simulator.apply(&game_state, &actions);

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
        expected_game_state.update_from_buffer(reader);

        assert_eq!(game_state, expected_game_state);

        let mut actions = std::collections::HashMap::new();
        actions.insert(1, action::Direction::Right);
        actions.insert(2, action::Direction::Up);
        actions.insert(3, action::Direction::Down);
        let game_state = simulator.apply(&game_state, &actions);

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
        expected_game_state.update_from_buffer(reader);

        assert_eq!(game_state, expected_game_state);

        let mut actions = std::collections::HashMap::new();
        actions.insert(1, action::Direction::Right);
        actions.insert(2, action::Direction::Up);
        actions.insert(3, action::Direction::Down);
        let game_state = simulator.apply(&game_state, &actions);

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
        expected_game_state.update_from_buffer(reader);

        assert_eq!(game_state, expected_game_state);

        let mut actions = std::collections::HashMap::new();
        actions.insert(1, action::Direction::Right);
        actions.insert(2, action::Direction::Up);
        actions.insert(3, action::Direction::Down);
        let game_state = simulator.apply(&game_state, &actions);

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
        expected_game_state.update_from_buffer(reader);

        assert_eq!(game_state, expected_game_state);

        let mut actions = std::collections::HashMap::new();
        actions.insert(1, action::Direction::Right);
        actions.insert(2, action::Direction::Up);
        actions.insert(3, action::Direction::Down);
        let game_state = simulator.apply(&game_state, &actions);

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
        expected_game_state.update_from_buffer(reader);

        assert_eq!(game_state, expected_game_state);

        let mut actions = std::collections::HashMap::new();
        actions.insert(1, action::Direction::Right);
        actions.insert(2, action::Direction::Up);
        actions.insert(3, action::Direction::Down);
        let game_state = simulator.apply(&game_state, &actions);

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
        expected_game_state.update_from_buffer(reader);

        assert_eq!(game_state, expected_game_state);

        let mut actions = std::collections::HashMap::new();
        actions.insert(1, action::Direction::Right);
        actions.insert(2, action::Direction::Up);
        let game_state = simulator.apply(&game_state, &actions);

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
        expected_game_state.update_from_buffer(reader);

        assert_eq!(game_state, expected_game_state);

        let mut actions = std::collections::HashMap::new();
        actions.insert(1, action::Direction::Right);
        actions.insert(2, action::Direction::Up);
        let game_state = simulator.apply(&game_state, &actions);

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
        expected_game_state.update_from_buffer(reader);

        assert_eq!(game_state, expected_game_state);

        let mut actions = std::collections::HashMap::new();
        actions.insert(1, action::Direction::Right);
        actions.insert(2, action::Direction::Up);
        let game_state = simulator.apply(&game_state, &actions);

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
        expected_game_state.update_from_buffer(reader);

        assert_eq!(game_state, expected_game_state);

        let mut actions = std::collections::HashMap::new();
        actions.insert(1, action::Direction::Right);
        actions.insert(2, action::Direction::Up);
        let game_state = simulator.apply(&game_state, &actions);

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
        expected_game_state.update_from_buffer(reader);

        assert_eq!(game_state, expected_game_state);

        let mut actions = std::collections::HashMap::new();
        actions.insert(1, action::Direction::Right);
        actions.insert(2, action::Direction::Up);
        let game_state = simulator.apply(&game_state, &actions);

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
        expected_game_state.update_from_buffer(reader);

        assert_eq!(game_state, expected_game_state);

        let mut actions = std::collections::HashMap::new();
        actions.insert(1, action::Direction::Right);
        actions.insert(2, action::Direction::Up);
        let game_state = simulator.apply(&game_state, &actions);

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
        expected_game_state.update_from_buffer(reader);

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
        let mut expected_platform_bitboard = bitboard::Bitboard::new(26);
        expected_platform_bitboard.turn_on(9, 2);
        expected_platform_bitboard.turn_on(16, 2);
        expected_platform_bitboard.turn_on(8, 3);
        expected_platform_bitboard.turn_on(17, 3);
        expected_platform_bitboard.turn_on(4, 4);
        expected_platform_bitboard.turn_on(5, 4);
        expected_platform_bitboard.turn_on(20, 4);
        expected_platform_bitboard.turn_on(21, 4);
        expected_platform_bitboard.turn_on(7, 5);
        expected_platform_bitboard.turn_on(18, 5);
        expected_platform_bitboard.turn_on(8, 6);
        expected_platform_bitboard.turn_on(9, 6);
        expected_platform_bitboard.turn_on(16, 6);
        expected_platform_bitboard.turn_on(17, 6);
        expected_platform_bitboard.turn_on(2, 7);
        expected_platform_bitboard.turn_on(23, 7);
        expected_platform_bitboard.turn_on(3, 8);
        expected_platform_bitboard.turn_on(22, 8);
        expected_platform_bitboard.turn_on(0, 9);
        expected_platform_bitboard.turn_on(1, 9);
        expected_platform_bitboard.turn_on(3, 9);
        expected_platform_bitboard.turn_on(4, 9);
        expected_platform_bitboard.turn_on(12, 9);
        expected_platform_bitboard.turn_on(13, 9);
        expected_platform_bitboard.turn_on(21, 9);
        expected_platform_bitboard.turn_on(22, 9);
        expected_platform_bitboard.turn_on(24, 9);
        expected_platform_bitboard.turn_on(25, 9);
        expected_platform_bitboard.turn_on(5, 10);
        expected_platform_bitboard.turn_on(6, 10);
        expected_platform_bitboard.turn_on(11, 10);
        expected_platform_bitboard.turn_on(12, 10);
        expected_platform_bitboard.turn_on(13, 10);
        expected_platform_bitboard.turn_on(14, 10);
        expected_platform_bitboard.turn_on(19, 10);
        expected_platform_bitboard.turn_on(20, 10);
        expected_platform_bitboard.turn_on(5, 11);
        expected_platform_bitboard.turn_on(6, 11);
        expected_platform_bitboard.turn_on(10, 11);
        expected_platform_bitboard.turn_on(11, 11);
        expected_platform_bitboard.turn_on(12, 11);
        expected_platform_bitboard.turn_on(13, 11);
        expected_platform_bitboard.turn_on(14, 11);
        expected_platform_bitboard.turn_on(15, 11);
        expected_platform_bitboard.turn_on(19, 11);
        expected_platform_bitboard.turn_on(20, 11);
        expected_platform_bitboard.turn_on(1, 12);
        expected_platform_bitboard.turn_on(4, 12);
        expected_platform_bitboard.turn_on(5, 12);
        expected_platform_bitboard.turn_on(6, 12);
        expected_platform_bitboard.turn_on(9, 12);
        expected_platform_bitboard.turn_on(10, 12);
        expected_platform_bitboard.turn_on(11, 12);
        expected_platform_bitboard.turn_on(12, 12);
        expected_platform_bitboard.turn_on(13, 12);
        expected_platform_bitboard.turn_on(14, 12);
        expected_platform_bitboard.turn_on(15, 12);
        expected_platform_bitboard.turn_on(16, 12);
        expected_platform_bitboard.turn_on(19, 12);
        expected_platform_bitboard.turn_on(20, 12);
        expected_platform_bitboard.turn_on(21, 12);
        expected_platform_bitboard.turn_on(24, 12);
        for x in 0..26 {
            expected_platform_bitboard.turn_on(x, 13);
        }
        assert_eq!(game_definition.platform_bitboard, expected_platform_bitboard);
        let mut expected_gravity_bitboard = bitboard::Bitboard::new(26);
        expected_gravity_bitboard.turn_on(9, 1);
        expected_gravity_bitboard.turn_on(16, 1);
        expected_gravity_bitboard.turn_on(8, 2);
        expected_gravity_bitboard.turn_on(17, 2);
        expected_gravity_bitboard.turn_on(4, 3);
        expected_gravity_bitboard.turn_on(5, 3);
        expected_gravity_bitboard.turn_on(20, 3);
        expected_gravity_bitboard.turn_on(21, 3);
        expected_gravity_bitboard.turn_on(7, 4);
        expected_gravity_bitboard.turn_on(18, 4);
        expected_gravity_bitboard.turn_on(8, 5);
        expected_gravity_bitboard.turn_on(9, 5);
        expected_gravity_bitboard.turn_on(16, 5);
        expected_gravity_bitboard.turn_on(17, 5);
        expected_gravity_bitboard.turn_on(2, 6);
        expected_gravity_bitboard.turn_on(23, 6);
        expected_gravity_bitboard.turn_on(3, 7);
        expected_gravity_bitboard.turn_on(22, 7);
        expected_gravity_bitboard.turn_on(0, 8);
        expected_gravity_bitboard.turn_on(1, 8);
        expected_gravity_bitboard.turn_on(3, 8);
        expected_gravity_bitboard.turn_on(4, 8);
        expected_gravity_bitboard.turn_on(12, 8);
        expected_gravity_bitboard.turn_on(13, 8);
        expected_gravity_bitboard.turn_on(21, 8);
        expected_gravity_bitboard.turn_on(22, 8);
        expected_gravity_bitboard.turn_on(24, 8);
        expected_gravity_bitboard.turn_on(25, 8);
        expected_gravity_bitboard.turn_on(5, 9);
        expected_gravity_bitboard.turn_on(6, 9);
        expected_gravity_bitboard.turn_on(11, 9);
        expected_gravity_bitboard.turn_on(12, 9);
        expected_gravity_bitboard.turn_on(13, 9);
        expected_gravity_bitboard.turn_on(14, 9);
        expected_gravity_bitboard.turn_on(19, 9);
        expected_gravity_bitboard.turn_on(20, 9);
        expected_gravity_bitboard.turn_on(5, 10);
        expected_gravity_bitboard.turn_on(6, 10);
        expected_gravity_bitboard.turn_on(10, 10);
        expected_gravity_bitboard.turn_on(11, 10);
        expected_gravity_bitboard.turn_on(12, 10);
        expected_gravity_bitboard.turn_on(13, 10);
        expected_gravity_bitboard.turn_on(14, 10);
        expected_gravity_bitboard.turn_on(15, 10);
        expected_gravity_bitboard.turn_on(19, 10);
        expected_gravity_bitboard.turn_on(20, 10);
        expected_gravity_bitboard.turn_on(1, 11);
        expected_gravity_bitboard.turn_on(4, 11);
        expected_gravity_bitboard.turn_on(5, 11);
        expected_gravity_bitboard.turn_on(6, 11);
        expected_gravity_bitboard.turn_on(9, 11);
        expected_gravity_bitboard.turn_on(10, 11);
        expected_gravity_bitboard.turn_on(11, 11);
        expected_gravity_bitboard.turn_on(12, 11);
        expected_gravity_bitboard.turn_on(13, 11);
        expected_gravity_bitboard.turn_on(14, 11);
        expected_gravity_bitboard.turn_on(15, 11);
        expected_gravity_bitboard.turn_on(16, 11);
        expected_gravity_bitboard.turn_on(19, 11);
        expected_gravity_bitboard.turn_on(20, 11);
        expected_gravity_bitboard.turn_on(21, 11);
        expected_gravity_bitboard.turn_on(24, 11);
        for x in 0..26 {
            expected_gravity_bitboard.turn_on(x, 12);
        }
        assert_eq!(game_definition.gravity_bitboard, expected_gravity_bitboard);
    }

    #[test]
    fn can_create_game_state() {
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

        let game_state = GameState::new(&game_definition);

        let expected_power_source_bitboard = bitboard::Bitboard::new(26);
        assert_eq!(game_state.power_source_bitboard, expected_power_source_bitboard);
        let mut expected_snake_id_to_snake_hash_map = std::collections::HashMap::new();
        for snake_id in 0..6 {
            expected_snake_id_to_snake_hash_map.insert(snake_id, snake::Snake::new(26));
        }
        assert_eq!(game_state.snake_id_to_snake, expected_snake_id_to_snake_hash_map);
    }

    #[test]
    fn can_update_game_state_from_buffer() {
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
            2
            0
            1
            3
            4
        "#;

        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);

        let game_definition = GameDefinition::from_buffer(reader);

        let mut game_state = GameState::new(&game_definition);

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

        game_state.update_from_buffer(reader);

        let mut expected_power_source_bitboard = bitboard::Bitboard::new(26);
        expected_power_source_bitboard.turn_on(0, 0);
        expected_power_source_bitboard.turn_on(24, 10);
        expected_power_source_bitboard.turn_on(1, 1);
        expected_power_source_bitboard.turn_on(13, 3);
        expected_power_source_bitboard.turn_on(8, 10);
        assert_eq!(game_state.power_source_bitboard, expected_power_source_bitboard);

        let mut expected_snake_id_to_snake: std::collections::HashMap<u32, snake::Snake> = std::collections::HashMap::new();
        let mut snake = snake::Snake::new(26);
        snake.add_body_part(0, 0);
        snake.add_body_part(1, 0);
        snake.add_body_part(1, 1);
        expected_snake_id_to_snake.insert(4, snake);

        let mut snake = snake::Snake::new(26);
        snake.add_body_part(20, 10);
        snake.add_body_part(19, 10);
        snake.add_body_part(19, 9);
        snake.add_body_part(19, 8);
        expected_snake_id_to_snake.insert(1, snake);

        let mut snake = snake::Snake::new(26);
        snake.add_body_part(10, 10);
        snake.add_body_part(11, 10);
        snake.add_body_part(12, 10);
        snake.add_body_part(13, 10);
        expected_snake_id_to_snake.insert(3, snake);

        let mut snake = snake::Snake::new(26);
        snake.add_body_part(5, 5);
        snake.add_body_part(6, 5);
        snake.add_body_part(7, 5);
        snake.add_body_part(8, 5);
        expected_snake_id_to_snake.insert(0, snake);

        assert_eq!(game_state.snake_id_to_snake, expected_snake_id_to_snake);
    }
}
