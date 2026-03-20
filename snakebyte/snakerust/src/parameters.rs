use crate::bitboard;

#[derive(Debug, PartialEq)]
pub struct Parameters {
    enemy_snake_ids: std::collections::HashSet<u8>,
    my_id: u8,
    my_snake_ids: std::collections::HashSet<u8>,
    grid_height: u16,
    grid_width: u16,
    platform_bitboard: bitboard::Bitboard,
}

impl Parameters {
    pub fn from_buffer<R: std::io::BufRead>(mut reader: R) -> Self {
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
            .parse::<u32>()
            .unwrap();

        let mut platform_bitboard = bitboard::Bitboard::new();
        let mut gravity_bitboard = bitboard::Bitboard::new();
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
                            platform_bitboard.turn_on(
                                bitboard::Bitboard::coord_to_index(
                                    x as u16,
                                    y as u16,
                                    grid_width
                                )
                            );
                        },
                        _ => panic!("grid should only contain '.' and '#'")
                    }
                })
        }

        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let snakebot_per_player_count = input_line
            .trim()
            .parse::<u8>()
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
                .parse::<u32>()
                .unwrap();

            enemy_snake_ids.insert(enemy_snake_id);
        }

        Self {
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

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn can_create_parameters_from_buffer() {
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

        let parameters = Parameters::from_buffer(reader);

        let mut expected_enemy_snake_ids = std::collections::HashSet::new();
        expected_enemy_snake_ids.insert(3);
        expected_enemy_snake_ids.insert(4);
        expected_enemy_snake_ids.insert(5);
        assert_eq!(parameters.enemy_snake_ids, expected_enemy_snake_ids);
        assert_eq!(parameters.my_id, 0);
        let mut expected_my_snake_ids = std::collections::HashSet::new();
        expected_my_snake_ids.insert(0);
        expected_my_snake_ids.insert(1);
        expected_my_snake_ids.insert(2);
        assert_eq!(parameters.my_snake_ids, expected_my_snake_ids);
        assert_eq!(parameters.grid_height, 14);
        assert_eq!(parameters.grid_width, 26);
        let mut expected_platform_bitboard = bitboard::Bitboard::new();
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
        assert_eq!(parameters.platform_bitboard, expected_platform_bitboard);
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
        assert_eq!(parameters.gravity_bitboard, expected_gravity_bitboard);
    }

}

