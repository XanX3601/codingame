use crate::action;
use crate::bitboard;

#[derive(Clone, Copy, Debug)]
pub struct Snake {
    body: [(i16, i16); Snake::BODY_MAX_LEN],
    body_bitboard: bitboard::Bitboard,
    direction: action::Direction,
    head_index: u8,
    tail_index: u8,
}

impl Snake {
    const BODY_MAX_LEN: usize = 256;

    pub fn add_body_part(&mut self, x: i16, y: i16, width: u16, is_in_grid: bool) {
        self.tail_index = self.tail_index.wrapping_sub(1);
        self.body[self.tail_index as usize] = (x, y);

        if is_in_grid{
            self.body_bitboard.turn_on(
                bitboard::Bitboard::coord_to_index(x as u16, y as u16, width)
            );
        }
    }

    pub fn clear(&mut self) {
        self.head_index = 0;
        self.tail_index = 0;
        self.body_bitboard.clear();
    }

    pub fn does_collide(&self, bitboard: &bitboard::Bitboard) -> bool {
        self.body_bitboard.does_collide(&bitboard)
    }

    pub fn does_head_collide(&self, bitboard: &bitboard::Bitboard) -> bool {
        // assume that snake is alway with at least one body part
        // as it should be removed from the game it is has 2 or less
        // assuming that, then head_index must be different than tail_index

        *head_x >= 0 && *head_x < self.body_bitboard.get_width() as i32 && *head_y >= 0 && bitboard.is_on(*head_x as u32, *head_y as u32)
    }

    pub fn get_body_bitboard(&self) -> &bitboard::Bitboard {
        &self.body_bitboard
    }

    pub fn get_head(&self) -> &(i32, i32) {
        match self.body.front() {
            Some(head) => head,
            None => panic!("attempt to get head of empty snake")
        }
    }

    pub fn grow(&mut self) {
        match self.tail_last_position {
            Some((tail_x, tail_y)) => {self.add_body_part(tail_x, tail_y);},
            None => {
                match self.body.back() {
                    Some((tail_x, tail_y)) => {
                        let (tail_x, tail_y) = action::Direction::Down.apply(*tail_x, *tail_y);
                        self.add_body_part(tail_x, tail_y);
                    }
                    None => panic!("attempt to grow an empty snake")
                };
            }
        }
    }

    pub fn len(&self) -> usize {
        self.body.len()
    }

    pub fn move_body_down(&mut self, step: i32) {
        self.body
            .iter_mut()
            .for_each(|(_, y)| {*y += step});

        self.tail_last_position = match self.tail_last_position {
            Some((x, y)) => Some((x, y + step)),
            None => None
        }
    }

    pub fn move_hitbox_down(&mut self) {
        self.body_bitboard.move_down_in_place();
    }

    pub fn move_same_direction(&mut self) {
        self.move_toward(self.direction);
    }

    pub fn move_toward(&mut self, mut direction: action::Direction) {
        // snake cannot move backward if so, it moves forward in the same
        // direction
        if self.direction.get_backward() == direction {
            direction = self.direction;
        }

        // to move a snake, remove its tail and move its head forward in the 
        // given direction
        let (tail_x, tail_y) = match self.body.pop_back() {
            Some(tail) => tail,
            None => panic!("attempt to move an empty snake"),
        };
        if tail_x >= 0 && tail_x < self.body_bitboard.get_width() as i32 && tail_y >= 0 {
            self.body_bitboard.turn_off(tail_x as u32, tail_y as u32);
        }
        self.tail_last_position = Some((tail_x, tail_y));

        let (head_x, head_y) = match self.body.front() {
            Some(head) => head,
            None => panic!("attempt to move an empty snake"),
        };
        let (new_head_x, new_head_y) = direction.apply(*head_x, *head_y);
        self.body.push_front((new_head_x, new_head_y));
        if new_head_x >= 0 && new_head_x < self.body_bitboard.get_width() as i32 && new_head_y >= 0 {
            self.body_bitboard.turn_on(new_head_x as u32, new_head_y as u32);
        }
        self.direction = direction;
    }

    pub fn new(width: u32) -> Snake {
        Snake {
            body: std::collections::VecDeque::new(),
            body_bitboard: bitboard::Bitboard::new(width),
            direction: action::Direction::Up,
            tail_last_position: None,
        }
    }

    pub fn remove_head(&mut self) {
        let (previous_head_x, previous_head_y) = match self.body.pop_front() {
            Some(previous_head) => previous_head,
            None => panic!("attempt to remove head of empty snake")
        };

        if previous_head_x >= 0 && previous_head_x < self.body_bitboard.get_width() as i32 && previous_head_y >= 0 {
            self.body_bitboard.turn_off(previous_head_x as u32, previous_head_y as u32);
        }
    }

    pub fn with_headless_body_bitboard<F, R>(&mut self, f: F) -> R
    where
        F: FnOnce(&bitboard::Bitboard) -> R
    {
        let (head_x, head_y) = match self.body.front() {
            Some(head) => head,
            None => panic!("attempting to do something to an empty snake"),
        };

        let must_turn_off_head = *head_x >= 0 && *head_x < self.body_bitboard.get_width() as i32 && *head_y >= 0;
        if must_turn_off_head {
            self.body_bitboard.turn_off(*head_x as u32, *head_y as u32);
        }

        let result = f(&self.body_bitboard);

        if must_turn_off_head {
            self.body_bitboard.turn_on(*head_x as u32, *head_y as u32);
        }

        result
    }
}

impl PartialEq for Snake {
    fn eq(&self, other: &Snake) -> bool {
        self.body == other.body
            && self.body_bitboard == other.body_bitboard
    }
}

#[cfg(test)]
mod test {
    use super::*;

    use rand::RngExt;
    
    #[test]
    fn can_add_body_parts() {
        let mut rng = rand::rng();
        let width: u32 = rng.random_range(15..46);

        let mut snake = Snake::new(width);
        snake.add_body_part(0, 0);
        snake.add_body_part(0, 1);
        snake.add_body_part(1, 1);
        snake.add_body_part(1, 0);

        assert_eq!(snake.body.len(), 4);

        let mut body_iter = snake.body.iter();
        assert_eq!(body_iter.next().unwrap(), &(0, 0));
        assert_eq!(body_iter.next().unwrap(), &(0, 1));
        assert_eq!(body_iter.next().unwrap(), &(1, 1));
        assert_eq!(body_iter.next().unwrap(), &(1, 0));

        let mut expected_body_bitboard = bitboard::Bitboard::new(width);
        expected_body_bitboard.turn_on(0, 0);
        expected_body_bitboard.turn_on(0, 1);
        expected_body_bitboard.turn_on(1, 1);
        expected_body_bitboard.turn_on(1, 0);

        assert_eq!(snake.body_bitboard, expected_body_bitboard);
    }

    #[test]
    fn can_clear_snake() {
        let mut rng = rand::rng();
        let width: u32 = rng.random_range(15..46);

        let mut snake = Snake::new(width);
        snake.add_body_part(0, 0);
        snake.add_body_part(0, 1);
        snake.add_body_part(1, 1);
        snake.add_body_part(1, 0);

        assert_eq!(snake.body.len(), 4);

        let mut expected_body_bitboard = bitboard::Bitboard::new(width);
        expected_body_bitboard.turn_on(0, 0);
        expected_body_bitboard.turn_on(0, 1);
        expected_body_bitboard.turn_on(1, 1);
        expected_body_bitboard.turn_on(1, 0);

        assert_eq!(snake.body_bitboard, expected_body_bitboard);

        snake.clear();

        assert_eq!(snake.body.len(), 0);

        assert_eq!(snake.body_bitboard, bitboard::Bitboard::new(width));
    }

    #[test]
    fn can_collide() {
        let mut snake = Snake::new(3);
        snake.add_body_part(1, 1);
        snake.add_body_part(0, 1);

        let mut bitboard = bitboard::Bitboard::new(3);

        for x in 0..3 {
            for y in 0..3 {
                bitboard.turn_on(x, y);

                if (x == 1 && y == 1) || (x == 0 && y == 1) {
                    assert!(snake.does_collide(&bitboard));
                    bitboard.turn_off(x, y);
                }
                else {
                    assert!(!snake.does_collide(&bitboard));
                }
            }
        }


        assert!(!snake.does_collide(&bitboard));
    }

    #[test]
    fn can_create_snake() {
        let mut rng = rand::rng();
        let width: u32 = rng.random_range(15..46);

        let _ = Snake::new(width);
    }

    #[test]
    fn can_grow() {
        let mut snake = Snake::new(6);
        snake.add_body_part(1, 2);
        snake.add_body_part(1, 3);
        snake.add_body_part(1, 4);

        assert_eq!(snake.tail_last_position, None);

        snake.grow();

        let mut expected_body = std::collections::VecDeque::new();
        expected_body.push_back((1, 2));
        expected_body.push_back((1, 3));
        expected_body.push_back((1, 4));
        expected_body.push_back((1, 5));

        assert_eq!(snake.body, expected_body);

        snake.move_toward(action::Direction::Right);

        assert_eq!(snake.tail_last_position, Some((1, 5)));

        snake.move_toward(action::Direction::Right);

        assert_eq!(snake.tail_last_position, Some((1, 4)));

        snake.move_toward(action::Direction::Right);

        assert_eq!(snake.tail_last_position, Some((1, 3)));

        snake.move_toward(action::Direction::Right);

        assert_eq!(snake.tail_last_position, Some((1, 2)));

        snake.move_toward(action::Direction::Up);

        assert_eq!(snake.tail_last_position, Some((2, 2)));

        snake.grow();

        let mut expected_body = std::collections::VecDeque::new();
        expected_body.push_back((5, 1));
        expected_body.push_back((5, 2));
        expected_body.push_back((4, 2));
        expected_body.push_back((3, 2));
        expected_body.push_back((2, 2));

        assert_eq!(snake.body, expected_body);

    }

    #[test]
    fn can_head_collide() {
        let mut snake = Snake::new(3);
        snake.add_body_part(1, 1);
        snake.add_body_part(0, 1);

        let mut bitboard = bitboard::Bitboard::new(3);

        for x in 0..3 {
            for y in 0..3 {
                bitboard.turn_on(x, y);

                if x == 1 && y == 1 {
                    assert!(snake.does_head_collide(&bitboard));
                    bitboard.turn_off(x, y);
                }
                else {
                    assert!(!snake.does_head_collide(&bitboard));
                }
            }
        }


        assert!(!snake.does_head_collide(&bitboard));
    }

    #[test]
    fn can_move_hitbox_and_body_down() {
        let mut snake = Snake::new(5);

        snake.add_body_part(2, 2);
        snake.add_body_part(2, 1);
        snake.add_body_part(1, 1);

        let mut expected_body = std::collections::VecDeque::new();
        expected_body.push_back((2, 2));
        expected_body.push_back((2, 1));
        expected_body.push_back((1, 1));

        let mut expected_body_bitboard = bitboard::Bitboard::new(5);
        expected_body_bitboard.turn_on(2, 2);
        expected_body_bitboard.turn_on(2, 1);
        expected_body_bitboard.turn_on(1, 1);

        assert_eq!(snake.body, expected_body);
        assert_eq!(snake.body_bitboard, expected_body_bitboard);

        snake.move_hitbox_down();

        let mut expected_body_bitboard = bitboard::Bitboard::new(5);
        expected_body_bitboard.turn_on(2, 3);
        expected_body_bitboard.turn_on(2, 2);
        expected_body_bitboard.turn_on(1, 2);

        assert_eq!(snake.body, expected_body);
        assert_eq!(snake.body_bitboard, expected_body_bitboard);

        snake.move_hitbox_down();

        let mut expected_body_bitboard = bitboard::Bitboard::new(5);
        expected_body_bitboard.turn_on(2, 4);
        expected_body_bitboard.turn_on(2, 3);
        expected_body_bitboard.turn_on(1, 3);

        assert_eq!(snake.body, expected_body);
        assert_eq!(snake.body_bitboard, expected_body_bitboard);

        snake.move_body_down(2);

        let mut expected_body = std::collections::VecDeque::new();
        expected_body.push_back((2, 4));
        expected_body.push_back((2, 3));
        expected_body.push_back((1, 3));

        assert_eq!(snake.body, expected_body);
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
    }

    #[test]
    fn can_move_toward() {
        let mut rng = rand::rng();
        let width: u32 = rng.random_range(15..46);

        let mut snake = Snake::new(width);
        snake.add_body_part(1, 1);
        snake.add_body_part(1, 2);
        snake.add_body_part(2, 2);
        snake.add_body_part(3, 2);
        snake.add_body_part(3, 3);

        snake.move_toward(action::Direction::Left);

        let mut expected_body: std::collections::VecDeque<(i32, i32)> = std::collections::VecDeque::new();
        expected_body.push_back((0, 1));
        expected_body.push_back((1, 1));
        expected_body.push_back((1, 2));
        expected_body.push_back((2, 2));
        expected_body.push_back((3, 2));

        let mut expected_body_bitboard = bitboard::Bitboard::new(width);
        expected_body_bitboard.turn_on(0, 1);
        expected_body_bitboard.turn_on(1, 1);
        expected_body_bitboard.turn_on(1, 2);
        expected_body_bitboard.turn_on(2, 2);
        expected_body_bitboard.turn_on(3, 2);

        assert_eq!(snake.body, expected_body);
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
        assert_eq!(snake.direction, action::Direction::Left);
        assert_eq!(snake.tail_last_position, Some((3, 3)));

        snake.move_toward(action::Direction::Right);

        let mut expected_body: std::collections::VecDeque<(i32, i32)> = std::collections::VecDeque::new();
        expected_body.push_back((-1, 1));
        expected_body.push_back((0, 1));
        expected_body.push_back((1, 1));
        expected_body.push_back((1, 2));
        expected_body.push_back((2, 2));

        let mut expected_body_bitboard = bitboard::Bitboard::new(width);
        expected_body_bitboard.turn_on(0, 1);
        expected_body_bitboard.turn_on(1, 1);
        expected_body_bitboard.turn_on(1, 2);
        expected_body_bitboard.turn_on(2, 2);

        assert_eq!(snake.body, expected_body);
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
        assert_eq!(snake.direction, action::Direction::Left);
        assert_eq!(snake.tail_last_position, Some((3, 2)));

        snake.move_toward(action::Direction::Up);

        let mut expected_body: std::collections::VecDeque<(i32, i32)> = std::collections::VecDeque::new();
        expected_body.push_back((-1, 0));
        expected_body.push_back((-1, 1));
        expected_body.push_back((0, 1));
        expected_body.push_back((1, 1));
        expected_body.push_back((1, 2));

        let mut expected_body_bitboard = bitboard::Bitboard::new(width);
        expected_body_bitboard.turn_on(0, 1);
        expected_body_bitboard.turn_on(1, 1);
        expected_body_bitboard.turn_on(1, 2);

        assert_eq!(snake.body, expected_body);
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
        assert_eq!(snake.direction, action::Direction::Up);
        assert_eq!(snake.tail_last_position, Some((2, 2)));

        snake.move_toward(action::Direction::Right);

        let mut expected_body: std::collections::VecDeque<(i32, i32)> = std::collections::VecDeque::new();
        expected_body.push_back((0, 0));
        expected_body.push_back((-1, 0));
        expected_body.push_back((-1, 1));
        expected_body.push_back((0, 1));
        expected_body.push_back((1, 1));

        let mut expected_body_bitboard = bitboard::Bitboard::new(width);
        expected_body_bitboard.turn_on(0, 0);
        expected_body_bitboard.turn_on(0, 1);
        expected_body_bitboard.turn_on(1, 1);

        assert_eq!(snake.body, expected_body);
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
        assert_eq!(snake.direction, action::Direction::Right);
        assert_eq!(snake.tail_last_position, Some((1, 2)));

        snake.move_toward(action::Direction::Down);

        let mut expected_body: std::collections::VecDeque<(i32, i32)> = std::collections::VecDeque::new();
        expected_body.push_back((0, 1));
        expected_body.push_back((0, 0));
        expected_body.push_back((-1, 0));
        expected_body.push_back((-1, 1));
        expected_body.push_back((0, 1));

        let mut expected_body_bitboard = bitboard::Bitboard::new(width);
        expected_body_bitboard.turn_on(0, 1);
        expected_body_bitboard.turn_on(0, 0);

        assert_eq!(snake.body, expected_body);
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
        assert_eq!(snake.direction, action::Direction::Down);
        assert_eq!(snake.tail_last_position, Some((1, 1)));
    }

    #[test]
    fn can_move_outside() {
        let mut snake = Snake::new(18);
        snake.add_body_part(17, 1);
        snake.add_body_part(16, 1);
        snake.add_body_part(15, 1);
        snake.add_body_part(15, 2);

        snake.move_toward(action::Direction::Right);

        let mut expected_body_parts = std::collections::VecDeque::new();
        expected_body_parts.push_back((18, 1));
        expected_body_parts.push_back((17, 1));
        expected_body_parts.push_back((16, 1));
        expected_body_parts.push_back((15, 1));

        let mut expected_body_bitboard = bitboard::Bitboard::new(18);
        expected_body_bitboard.turn_on(17, 1);
        expected_body_bitboard.turn_on(16, 1);
        expected_body_bitboard.turn_on(15, 1);

        assert_eq!(snake.body, expected_body_parts);
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
    }

    #[test]
    fn can_remove_head() {
        let mut snake = Snake::new(10);
        snake.add_body_part(-1, -1);
        snake.add_body_part(0, -1);
        snake.add_body_part(0, 0);
        snake.add_body_part(1, 0);
        snake.add_body_part(1, 1);

        let mut expected_body = std::collections::VecDeque::new();
        expected_body.push_back((-1, -1));
        expected_body.push_back((0, -1));
        expected_body.push_back((0, 0));
        expected_body.push_back((1, 0));
        expected_body.push_back((1, 1));

        let mut expected_body_bitboard = bitboard::Bitboard::new(10);
        expected_body_bitboard.turn_on(0, 0);
        expected_body_bitboard.turn_on(1, 0);
        expected_body_bitboard.turn_on(1, 1);

        assert_eq!(snake.body, expected_body);
        assert_eq!(snake.body_bitboard, expected_body_bitboard);

        snake.remove_head();

        expected_body.pop_front();

        assert_eq!(snake.body, expected_body);
        assert_eq!(snake.body_bitboard, expected_body_bitboard);

        snake.remove_head();

        expected_body.pop_front();

        assert_eq!(snake.body, expected_body);
        assert_eq!(snake.body_bitboard, expected_body_bitboard);

        snake.remove_head();

        expected_body.pop_front();
        expected_body_bitboard.turn_off(0, 0);

        assert_eq!(snake.body, expected_body);
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
    }
}
