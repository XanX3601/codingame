use crate::action;
use crate::bitboard;
use crate::zobrist;

#[derive(Clone, Copy, Debug)]
pub struct Snake {
    body: [(i16, i16); Self::BODY_MAX_LEN],
    body_bitboard: bitboard::Bitboard,
    direction: action::Direction,
    head_index: u8,
    tail_index: u8,
    zobrist_hash: u64,
}

impl Snake {
    const BODY_MAX_LEN: usize = 256;

    pub fn add_body_part(
        &mut self,
        x: i16,
        y: i16,
        width: u16,
        height: u16,
        zobrist_table: &zobrist::ZobristTable, snake_id: u8
    ) {
        self.body[self.tail_index as usize] = (x, y);
        self.tail_index = self.tail_index.wrapping_sub(1);

        if Self::is_in_grid(x, y, width, height) {
            self.body_bitboard.turn_on(
                bitboard::Bitboard::coord_to_index(x as u16, y as u16, width)
            );

            self.zobrist_hash ^= zobrist_table.get_snake_body_hash(
                snake_id,
                bitboard::Bitboard::coord_to_index(x as u16, y as u16, width)
            );
        }
    }

    pub fn clear(&mut self) {
        self.body = [(0, 0); Self::BODY_MAX_LEN];
        self.body_bitboard.clear();
        self.head_index = 0;
        self.tail_index = 0;
        self.zobrist_hash = 0;
    }

    pub fn does_collide(&self, bitboard: &bitboard::Bitboard) -> bool {
        self.body_bitboard.does_collide(&bitboard)
    }

    pub fn does_head_collide(&self, bitboard: &bitboard::Bitboard, width: u16, height: u16) -> bool {
        // assume it is not called on empty snake
        let (head_x, head_y) = self.body[self.head_index as usize];
        Self::is_in_grid(head_x, head_y, width, height)
            && bitboard.is_on(bitboard::Bitboard::coord_to_index(head_x as u16, head_y as u16, width))
    }

    pub fn get_body_bitboard(&self) -> &bitboard::Bitboard {
        &self.body_bitboard
    }

    pub fn get_direction(&self) -> action::Direction {
        self.direction
    }

    pub fn get_head(&self) -> (i16, i16) {
        self.body[self.head_index as usize]
    }

    pub fn get_zobrist_hash(&self) -> u64 {
        self.zobrist_hash
    }

    pub fn grow(&mut self, width: u16, height: u16, zobrist_table: &zobrist::ZobristTable, snake_id: u8) {
        let (new_tail_x, new_tail_y) = self.body[self.tail_index as usize];
        self.tail_index = self.tail_index.wrapping_sub(1);
        if Self::is_in_grid(new_tail_x, new_tail_y, width, height) {
            self.body_bitboard.turn_on(
                bitboard::Bitboard::coord_to_index(new_tail_x as u16, new_tail_y as u16, width)
            );
            self.zobrist_hash ^= zobrist_table.get_snake_body_hash(
                snake_id,
                bitboard::Bitboard::coord_to_index(new_tail_x as u16, new_tail_y as u16, width)
            );
        }
    }

    pub fn is_in_grid(x:i16, y:i16, width: u16, height:u16) -> bool {
        0 <= x && (x as u16) < width && 0 <= y && (y as u16) < height
    }

    pub fn len(&self) -> usize {
        (self.head_index.wrapping_sub(self.tail_index)) as usize
    }

    pub fn move_body_down(&mut self, step: i16, width: u16, height: u16, zobrist_table: &zobrist::ZobristTable, snake_id: u8) {
        let mut body_index = self.tail_index.wrapping_add(1);
        while body_index != self.head_index.wrapping_add(1) {
            let (body_x, body_y) = self.body[body_index as usize];
            if Snake::is_in_grid(body_x, body_y, width, height) {
                self.zobrist_hash ^= zobrist_table.get_snake_body_hash(
                    snake_id, 
                    bitboard::Bitboard::coord_to_index(body_x as u16, body_y as u16, width)
                );
            }

            let (body_x, body_y) = (body_x, body_y + step);
            if Snake::is_in_grid(body_x, body_y, width, height) {
                self.zobrist_hash ^= zobrist_table.get_snake_body_hash(
                    snake_id, 
                    bitboard::Bitboard::coord_to_index(body_x as u16, body_y as u16, width)
                );
            }

            self.body[body_index as usize] = (body_x, body_y);
            body_index = body_index.wrapping_add(1);
        }
    }

    pub fn move_hitbox_down(&mut self, width: u16) {
        self.body_bitboard.move_down_in_place(width);
    }

    pub fn move_same_direction(&mut self, width: u16, height: u16, zobrist_table: &zobrist::ZobristTable, snake_id: u8) {
        self.move_toward(self.direction, width, height, &zobrist_table, snake_id);
    }

    pub fn move_toward(&mut self, mut direction: action::Direction, width: u16, height: u16, zobrist_table: &zobrist::ZobristTable, snake_id: u8) {
        // snake cannot move backward if so, it moves forward in the same
        // direction
        if self.direction.get_backward() == direction {
            direction = self.direction;
        }

        // to move a snake, remove its tail and move its head forward in the 
        // given direction
        self.tail_index = self.tail_index.wrapping_add(1);
        let (tail_x, tail_y) = self.body[self.tail_index as usize];
        if Self::is_in_grid(tail_x, tail_y, width, height) {
            self.body_bitboard.turn_off(
                bitboard::Bitboard::coord_to_index(tail_x as u16, tail_y as u16, width)
            );
            self.zobrist_hash ^= zobrist_table.get_snake_body_hash(
                snake_id, 
                bitboard::Bitboard::coord_to_index(tail_x as u16, tail_y as u16, width)
            );
        }

        let (head_x, head_y) = self.body[self.head_index as usize];
        let (new_head_x, new_head_y) = direction.apply(head_x, head_y);
        self.head_index = self.head_index.wrapping_add(1);
        self.body[self.head_index as usize] = (new_head_x, new_head_y);
        if Self::is_in_grid(new_head_x, new_head_y, width, height) {
            self.body_bitboard.turn_on(
                bitboard::Bitboard::coord_to_index(new_head_x as u16, new_head_y as u16, width)
            );
            self.zobrist_hash ^= zobrist_table.get_snake_body_hash(
                snake_id, 
                bitboard::Bitboard::coord_to_index(new_head_x as u16, new_head_y as u16, width)
            );
        }
        self.direction = direction;
    }

    pub fn new() -> Self {
        Self {
            body: [(0, 0); Self::BODY_MAX_LEN],
            body_bitboard: bitboard::Bitboard::new(),
            direction: action::Direction::Up,
            head_index: 0,
            tail_index: 0,
            zobrist_hash: 0,
        }
    }

    pub fn remove_head(&mut self, width: u16, height: u16, zobrist_table: &zobrist::ZobristTable, snake_id: u8) {
        let (previous_head_x, previous_head_y) = self.body[self.head_index as usize];
        self.head_index = self.head_index.wrapping_sub(1);

        if Self::is_in_grid(previous_head_x, previous_head_y, width, height) {
            self.body_bitboard.turn_off(
                bitboard::Bitboard::coord_to_index(previous_head_x as u16, previous_head_y as u16, width)
            );
            self.zobrist_hash ^= zobrist_table.get_snake_body_hash(
                snake_id, 
                bitboard::Bitboard::coord_to_index(previous_head_x as u16, previous_head_y as u16, width)
            );
        }
    }

    pub fn with_headless_body_bitboard<F, R>(&mut self, width: u16, height: u16, f: F) -> R
    where
        F: FnOnce(&bitboard::Bitboard) -> R
    {
        let (head_x, head_y) = self.body[self.head_index as usize];

        let is_head_in_grid = Self::is_in_grid(head_x, head_y, width, height);
        if is_head_in_grid {
            self.body_bitboard.turn_off(
                bitboard::Bitboard::coord_to_index(head_x as u16, head_y as u16, width)
            );
        }

        let result = f(&self.body_bitboard);

        if is_head_in_grid {
            self.body_bitboard.turn_on(
                bitboard::Bitboard::coord_to_index(head_x as u16, head_y as u16, width)
            );
        }

        result
    }
}

impl PartialEq for Snake {
    fn eq(&self, other: &Self) -> bool {
        if self.len() != other.len() { return false; }
        
        let mut body_index = self.tail_index.wrapping_add(1);
        let mut other_body_index = other.tail_index.wrapping_add(1);
        while body_index != self.head_index.wrapping_add(1) {
            if self.body[body_index as usize] != other.body[other_body_index as usize] {
                return false;
            }
            body_index = body_index.wrapping_add(1);
            other_body_index = other_body_index.wrapping_add(1);
        }

        self.zobrist_hash == other.zobrist_hash
            && self.body_bitboard == other.body_bitboard
    }
}

#[cfg(test)]
mod test {
    use super::*;
    use pretty_assertions::assert_eq;

    use rand::RngExt;

    fn check_body(snake: &Snake, expected_body: &Vec<(i16, i16)>) -> bool {
        let mut body_index = snake.tail_index.wrapping_add(1);

        let mut expected_body_index = 0;
        while body_index != snake.head_index.wrapping_add(1) {
            if snake.body[body_index as usize] != expected_body[expected_body_index] {
                return false;
            }
            expected_body_index += 1;
            body_index = body_index.wrapping_add(1);
        }

        body_index == snake.head_index.wrapping_add(1)
            && expected_body_index == expected_body.len()
    }
    
    #[test]
    fn can_add_body_parts() {
        let zobrist_table = zobrist::ZobristTable::new();
        let snake_id = 0;

        let mut rng = rand::rng();
        let width: u16 = rng.random_range(15..46);
        let height: u16 = rng.random_range(10..31);

        let mut snake = Snake::new();
        snake.add_body_part(0, 0, width, height, &zobrist_table, snake_id);
        snake.add_body_part(0, 1, width, height, &zobrist_table, snake_id);
        snake.add_body_part(1, 1, width, height, &zobrist_table, snake_id);
        snake.add_body_part(1, 0, width, height, &zobrist_table, snake_id);

        assert_eq!(snake.len(), 4);

        let mut expected_body = Vec::new();
        expected_body.push((1, 0));
        expected_body.push((1, 1));
        expected_body.push((0, 1));
        expected_body.push((0, 0));

        assert!(check_body(&snake, &expected_body));

        let mut expected_body_bitboard = bitboard::Bitboard::new();
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(0, 0, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(0, 1, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 1, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 0, width));

        assert_eq!(snake.body_bitboard, expected_body_bitboard);
    }

    #[test]
    fn can_clear_snake() {
        let zobrist_table = zobrist::ZobristTable::new();
        let snake_id = 0;

        let mut rng = rand::rng();
        let width: u16 = rng.random_range(15..46);
        let height: u16 = rng.random_range(10..31);

        let mut snake = Snake::new();
        snake.add_body_part(0, 0, width, height, &zobrist_table, snake_id);
        snake.add_body_part(0, 1, width, height, &zobrist_table, snake_id);
        snake.add_body_part(1, 1, width, height, &zobrist_table, snake_id);
        snake.add_body_part(1, 0, width, height, &zobrist_table, snake_id);

        assert_eq!(snake.len(), 4);

        let mut expected_body_bitboard = bitboard::Bitboard::new();
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(0, 0, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(0, 1, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 1, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 0, width));

        assert_eq!(snake.body_bitboard, expected_body_bitboard);

        snake.clear();

        assert_eq!(snake.len(), 0);
        assert_eq!(snake.body_bitboard, bitboard::Bitboard::new());
    }

    #[test]
    fn can_collide() {
        let zobrist_table = zobrist::ZobristTable::new();
        let snake_id = 0;

        let width = 3;
        let height = width;

        let mut snake = Snake::new();
        snake.add_body_part(1, 1, width, height, &zobrist_table, snake_id);
        snake.add_body_part(0, 1, width, height, &zobrist_table, snake_id);

        let mut bitboard = bitboard::Bitboard::new();

        for x in 0..width {
            for y in 0..height {
                bitboard.turn_on(bitboard::Bitboard::coord_to_index(x, y, width));

                if (x == 1 && y == 1) || (x == 0 && y == 1) {
                    assert!(snake.does_collide(&bitboard));
                    bitboard.turn_off(bitboard::Bitboard::coord_to_index(x, y, width));
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
        let _ = Snake::new();
    }

    #[test]
    fn can_grow() {
        let zobrist_table = zobrist::ZobristTable::new();
        let snake_id = 0;

        let width = 6;
        let height = width;

        let mut snake = Snake::new();
        snake.add_body_part(1, 2, width, height, &zobrist_table, snake_id);
        snake.add_body_part(1, 3, width, height, &zobrist_table, snake_id);
        snake.add_body_part(1, 4, width, height, &zobrist_table, snake_id);

        snake.grow(width, height, &zobrist_table, snake_id);

        let mut expected_body = Vec::new();
        expected_body.push((1, 2));
        expected_body.push((1, 3));
        expected_body.push((1, 4));
        expected_body.push((0, 0));
        expected_body.reverse();

        assert!(check_body(&snake, &expected_body));

        snake.move_toward(action::Direction::Right, width, height, &zobrist_table, snake_id);
        snake.move_toward(action::Direction::Right, width, height, &zobrist_table, snake_id);
        snake.move_toward(action::Direction::Right, width, height, &zobrist_table, snake_id);
        snake.move_toward(action::Direction::Right, width, height, &zobrist_table, snake_id);
        snake.move_toward(action::Direction::Up, width, height, &zobrist_table, snake_id);

        snake.grow(width, height, &zobrist_table, snake_id);

        let mut expected_body = Vec::new();
        expected_body.push((5, 1));
        expected_body.push((5, 2));
        expected_body.push((4, 2));
        expected_body.push((3, 2));
        expected_body.push((2, 2));
        expected_body.reverse();

        assert!(check_body(&snake, &expected_body));
    }

    #[test]
    fn can_head_collide() {
        let zobrist_table = zobrist::ZobristTable::new();
        let snake_id = 0;

        let width = 3;
        let height = width;

        let mut snake = Snake::new();
        snake.add_body_part(1, 1, width, height, &zobrist_table, snake_id);
        snake.add_body_part(0, 1, width, height, &zobrist_table, snake_id);

        let mut bitboard = bitboard::Bitboard::new();

        for x in 0..3 {
            for y in 0..3 {
                bitboard.turn_on(bitboard::Bitboard::coord_to_index(x, y, width));

                if x == 1 && y == 1 {
                    assert!(snake.does_head_collide(&bitboard, width, height));
                    bitboard.turn_off(bitboard::Bitboard::coord_to_index(x, y, width));
                }
                else {
                    assert!(!snake.does_head_collide(&bitboard, width, height));
                }
            }
        }

        assert!(!snake.does_head_collide(&bitboard, width, height));
    }

    #[test]
    fn can_move_hitbox_and_body_down() {
        let zobrist_table = zobrist::ZobristTable::new();
        let snake_id = 0;

        let width = 5;
        let height = width;

        let mut snake = Snake::new();

        snake.add_body_part(2, 2, width, height, &zobrist_table, snake_id);
        snake.add_body_part(2, 1, width, height, &zobrist_table, snake_id);
        snake.add_body_part(1, 1, width, height, &zobrist_table, snake_id);

        let mut expected_body = Vec::new();
        expected_body.push((2, 2));
        expected_body.push((2, 1));
        expected_body.push((1, 1));
        expected_body.reverse();

        let mut expected_body_bitboard = bitboard::Bitboard::new();
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(2, 2, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(2, 1, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 1, width));

        assert!(check_body(&snake, &expected_body));
        assert_eq!(snake.body_bitboard, expected_body_bitboard);

        snake.move_hitbox_down(width);

        let mut expected_body_bitboard = bitboard::Bitboard::new();
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(2, 3, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(2, 2, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 2, width));

        assert!(check_body(&snake, &expected_body));
        assert_eq!(snake.body_bitboard, expected_body_bitboard);

        snake.move_hitbox_down(width);

        let mut expected_body_bitboard = bitboard::Bitboard::new();
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(2, 4, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(2, 3, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 3, width));

        assert!(check_body(&snake, &expected_body));
        assert_eq!(snake.body_bitboard, expected_body_bitboard);

        snake.move_body_down(2, width, height, &zobrist_table, snake_id);

        let mut expected_body = Vec::new();
        expected_body.push((2, 4));
        expected_body.push((2, 3));
        expected_body.push((1, 3));
        expected_body.reverse();

        assert!(check_body(&snake, &expected_body));
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
    }

    #[test]
    fn can_move_toward() {
        let zobrist_table = zobrist::ZobristTable::new();
        let snake_id = 0;

        let mut rng = rand::rng();
        let width: u16 = rng.random_range(15..46);
        let height: u16 = rng.random_range(10..31);

        let mut snake = Snake::new();
        snake.add_body_part(1, 1, width, height, &zobrist_table, snake_id);
        snake.add_body_part(1, 2, width, height, &zobrist_table, snake_id);
        snake.add_body_part(2, 2, width, height, &zobrist_table, snake_id);
        snake.add_body_part(3, 2, width, height, &zobrist_table, snake_id);
        snake.add_body_part(3, 3, width, height, &zobrist_table, snake_id);

        snake.move_toward(action::Direction::Left, width, height, &zobrist_table, snake_id);

        let mut expected_body = Vec::new();
        expected_body.push((0, 1));
        expected_body.push((1, 1));
        expected_body.push((1, 2));
        expected_body.push((2, 2));
        expected_body.push((3, 2));
        expected_body.reverse();

        let mut expected_body_bitboard = bitboard::Bitboard::new();
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(0, 1, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 1, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 2, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(2, 2, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(3, 2, width));

        assert!(check_body(&snake, &expected_body));
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
        assert_eq!(snake.direction, action::Direction::Left);

        snake.move_toward(action::Direction::Right, width, height, &zobrist_table, snake_id);

        let mut expected_body = Vec::new();
        expected_body.push((-1, 1));
        expected_body.push((0, 1));
        expected_body.push((1, 1));
        expected_body.push((1, 2));
        expected_body.push((2, 2));
        expected_body.reverse();

        let mut expected_body_bitboard = bitboard::Bitboard::new();
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(0, 1, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 1, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 2, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(2, 2, width));

        assert!(check_body(&snake, &expected_body));
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
        assert_eq!(snake.direction, action::Direction::Left);

        snake.move_toward(action::Direction::Up, width, height, &zobrist_table, snake_id);

        let mut expected_body = Vec::new();
        expected_body.push((-1, 0));
        expected_body.push((-1, 1));
        expected_body.push((0, 1));
        expected_body.push((1, 1));
        expected_body.push((1, 2));
        expected_body.reverse();

        let mut expected_body_bitboard = bitboard::Bitboard::new();
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(0, 1, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 1, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 2, width));

        assert!(check_body(&snake, &expected_body));
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
        assert_eq!(snake.direction, action::Direction::Up);

        snake.move_toward(action::Direction::Right, width, height, &zobrist_table, snake_id);

        let mut expected_body = Vec::new();
        expected_body.push((0, 0));
        expected_body.push((-1, 0));
        expected_body.push((-1, 1));
        expected_body.push((0, 1));
        expected_body.push((1, 1));
        expected_body.reverse();

        let mut expected_body_bitboard = bitboard::Bitboard::new();
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(0, 0, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(0, 1, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 1, width));

        assert!(check_body(&snake, &expected_body));
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
        assert_eq!(snake.direction, action::Direction::Right);

        snake.move_toward(action::Direction::Down, width, height, &zobrist_table, snake_id);

        let mut expected_body = Vec::new();
        expected_body.push((0, 1));
        expected_body.push((0, 0));
        expected_body.push((-1, 0));
        expected_body.push((-1, 1));
        expected_body.push((0, 1));
        expected_body.reverse();

        let mut expected_body_bitboard = bitboard::Bitboard::new();
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(0, 1, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(0, 0, width));

        assert!(check_body(&snake, &expected_body));
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
        assert_eq!(snake.direction, action::Direction::Down);
    }

    #[test]
    fn can_move_outside() {
        let zobrist_table = zobrist::ZobristTable::new();
        let snake_id = 0;

        let width = 18;
        let height = width;

        let mut snake = Snake::new();
        snake.add_body_part(17, 1, width, height, &zobrist_table, snake_id);
        snake.add_body_part(16, 1, width, height, &zobrist_table, snake_id);
        snake.add_body_part(15, 1, width, height, &zobrist_table, snake_id);
        snake.add_body_part(15, 2, width, height, &zobrist_table, snake_id);

        snake.move_toward(action::Direction::Right, width, height, &zobrist_table, snake_id);

        let mut expected_body_parts = Vec::new();
        expected_body_parts.push((18, 1));
        expected_body_parts.push((17, 1));
        expected_body_parts.push((16, 1));
        expected_body_parts.push((15, 1));
        expected_body_parts.reverse();

        let mut expected_body_bitboard = bitboard::Bitboard::new();
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(17, 1, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(16, 1, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(15, 1, width));

        assert!(check_body(&snake, &expected_body_parts));
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
    }

    #[test]
    fn can_remove_head() {
        let zobrist_table = zobrist::ZobristTable::new();
        let snake_id = 0;

        let width = 10;
        let height = width;

        let mut snake = Snake::new();
        snake.add_body_part(-1, -1, width, height, &zobrist_table, snake_id);
        snake.add_body_part(0, -1, width, height, &zobrist_table, snake_id);
        snake.add_body_part(0, 0, width, height, &zobrist_table, snake_id);
        snake.add_body_part(1, 0, width, height, &zobrist_table, snake_id);
        snake.add_body_part(1, 1, width, height, &zobrist_table, snake_id);

        let mut expected_body = Vec::new();
        expected_body.push((-1, -1));
        expected_body.push((0, -1));
        expected_body.push((0, 0));
        expected_body.push((1, 0));
        expected_body.push((1, 1));
        expected_body.reverse();

        let mut expected_body_bitboard = bitboard::Bitboard::new();
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(0, 0, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 0, width));
        expected_body_bitboard.turn_on(bitboard::Bitboard::coord_to_index(1, 1, width));

        assert!(check_body(&snake, &expected_body));
        assert_eq!(snake.body_bitboard, expected_body_bitboard);

        snake.remove_head(width, height, &zobrist_table, snake_id);

        let mut expected_body = Vec::new();
        expected_body.push((0, -1));
        expected_body.push((0, 0));
        expected_body.push((1, 0));
        expected_body.push((1, 1));
        expected_body.reverse();

        assert!(check_body(&snake, &expected_body));
        assert_eq!(snake.body_bitboard, expected_body_bitboard);

        snake.remove_head(width, height, &zobrist_table, snake_id);

        let mut expected_body = Vec::new();
        expected_body.push((0, 0));
        expected_body.push((1, 0));
        expected_body.push((1, 1));
        expected_body.reverse();

        assert!(check_body(&snake, &expected_body));
        assert_eq!(snake.body_bitboard, expected_body_bitboard);

        snake.remove_head(width, height, &zobrist_table, snake_id);

        let mut expected_body = Vec::new();
        expected_body.push((1, 0));
        expected_body.push((1, 1));
        expected_body.reverse();

        expected_body_bitboard.turn_off(bitboard::Bitboard::coord_to_index(0, 0, width));

        assert!(check_body(&snake, &expected_body));
        assert_eq!(snake.body_bitboard, expected_body_bitboard);
    }
}
