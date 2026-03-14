use crate::bitboard;
use crate::snake;

#[derive(Debug)]
pub struct GameDefinition {
    enemy_snake_ids: std::collections::HashSet<u32>,
    my_id: u32,
    my_snake_ids: std::collections::HashSet<u32>,
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
                        '#' => platform_bitboard.turn_on(x as u32, y),
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
            grid_height: grid_height,
            grid_width: grid_width,
            platform_bitboard: platform_bitboard,
        }
    }
}

#[derive(Clone)]
pub struct GameState<'a> {
    game_definition: &'a GameDefinition,
    power_source_bitboard: bitboard::Bitboard,
    snake_id_to_snake: std::collections::HashMap<u32, snake::Snake>,
}

impl<'a> GameState<'a> {
    pub fn new(game_definition: &'a GameDefinition) -> GameState<'a> {
        let mut snake_id_to_snake: std::collections::HashMap<u32, snake::Snake> = std::collections::HashMap::new();
        for snake_id in &game_definition.enemy_snake_ids {
            snake_id_to_snake.insert(*snake_id, snake::Snake{});
        }
        for snake_id in &game_definition.my_snake_ids {
            snake_id_to_snake.insert(*snake_id, snake::Snake{});
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

        for _ in 0..snakebot_count {
            let mut input_line = String::new();

        }
    }
}

#[cfg(test)]
mod test {
    use super::*;

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
    }

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
            expected_snake_id_to_snake_hash_map.insert(snake_id, snake::Snake{});
        }
        assert_eq!(game_state.snake_id_to_snake, expected_snake_id_to_snake_hash_map);
    }
}
