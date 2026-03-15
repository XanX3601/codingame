use crate::bitboard;

#[derive(Clone, Debug, PartialEq)]
pub struct Bitboard {
    bits: [u64; 22],
    width: u32,
}

impl Bitboard {
    pub fn clear(&mut self) {
        for bit in &mut self.bits {
            *bit &= 0;
        }
    }

    pub fn does_collide(&self, bitboard: &Bitboard) -> bool {
        for i in 0..self.bits.len() {
            if self.bits[i] & bitboard.bits[i] > 0{
                return true;
            }
        }
        
        false
    }

    pub fn is_on(&self, x: u32, y: u32) -> bool {
        let bit_index = (y * self.width + x) as usize;
        (self.bits[bit_index / 64] >> (bit_index % 64)) & 1 == 1
    }

    pub fn move_down(&mut self) {
        let inv_width = 64 - self.width;

        for i in (0..self.bits.len()).rev() {
            self.bits[i] <<= self.width;

            if i > 0 {
                self.bits[i] |= self.bits[i - 1] >> inv_width;
            }
        }
    }

    pub fn move_up(&mut self) {
        let inv_width = 64 - self.width;

        for i in 0..self.bits.len() {
            self.bits[i] >>= self.width;

            if i + 1 < self.bits.len() {
                self.bits[i] |= self.bits[i + 1] << inv_width;
            }
        }
    }

    pub fn new(width: u32) -> Bitboard {
        Bitboard {
            bits: [0; 22],
            width: width,
        }
    }

    pub fn or(&mut self, bitboard: &Bitboard) {
        for i in 0..self.bits.len() {
            self.bits[i] |= bitboard.bits[i];
        }
    }

    pub fn toggle(&mut self, x: u32, y: u32) {
        let bit_index = (y * self.width + x) as usize;
        self.bits[bit_index / 64] ^= 1 << (bit_index % 64);
    }

    pub fn turn_on(&mut self, x: u32, y: u32) {
        let bit_index = (y * self.width + x) as usize;
        self.bits[bit_index / 64] |= 1 << (bit_index % 64);
    }

    pub fn turn_off(&mut self, x: u32, y: u32) {
        let bit_index = (y * self.width + x) as usize;
        self.bits[bit_index / 64] &= !(1 << (bit_index % 64));
    }
}

#[cfg(test)]
mod test {
    use crate::bitboard;

    use super::*;

    use rand::RngExt;

    #[test]
    fn can_clear() {
        let mut rng = rand::rng();

        let width: u32 = rng.random_range(15..46);
        let height: u32 = rng.random_range(10..31);

        let mut bitboard = Bitboard::new(width);

        for _ in 0..10 {
            let x: u32 = rng.random_range(0..width);
            let y: u32 = rng.random_range(0..height);

            bitboard.turn_on(x, y);
        }

        bitboard.clear();

        for x in 0..width {
            for y in 0..height {
                assert!(!bitboard.is_on(x, y));
            }
        }
    }

    #[test]
    fn can_collide_with() {
        let mut bitboard = Bitboard::new(3);
        bitboard.turn_on(0, 0);
        bitboard.turn_on(2, 0);
        bitboard.turn_on(1, 1);
        bitboard.turn_on(0, 2);
        bitboard.turn_on(2, 2);

        let mut other_bitboard = Bitboard::new(3);
        other_bitboard.turn_on(1, 0);
        other_bitboard.turn_on(0, 1);
        other_bitboard.turn_on(2, 1);
        other_bitboard.turn_on(1, 2);

        assert!(!bitboard.does_collide(&other_bitboard));
        assert!(!other_bitboard.does_collide(&bitboard));

        assert!(bitboard.does_collide(&bitboard));
        assert!(other_bitboard.does_collide(&other_bitboard));

        bitboard.turn_on(1, 0);

        assert!(bitboard.does_collide(&other_bitboard));
        assert!(other_bitboard.does_collide(&bitboard));
    }

    #[test]
    fn can_create_one() {
        let _ = Bitboard::new(15);
    }

    #[test]
    fn can_move_down() {
        let mut bitboard = bitboard::Bitboard::new(5);

        bitboard.toggle(0, 0);

        for x in 0..5 {
            for y in 0..5 {
                if x == 0 && y == 0 {
                    assert!(bitboard.is_on(x, y));
                }
                else {
                    assert!(!bitboard.is_on(x, y));
                }
            }
        }

        for x in 0..5 {
            bitboard.turn_on(x, 4);
        }

        bitboard.move_down();

        for x in 0..5 {
            for y in 0..5 {
                if x == 0 && y == 1 {
                    assert!(bitboard.is_on(x, y));
                }
                else {
                    assert!(!bitboard.is_on(x, y));
                }
            }
        }

        bitboard.move_down();

        for x in 0..5 {
            for y in 0..5 {
                if x == 0 && y == 2 {
                    assert!(bitboard.is_on(x, y));
                }
                else {
                    assert!(!bitboard.is_on(x, y));
                }
            }
        }

        bitboard.move_down();

        for x in 0..5 {
            for y in 0..5 {
                if x == 0 && y == 3 {
                    assert!(bitboard.is_on(x, y));
                }
                else {
                    assert!(!bitboard.is_on(x, y));
                }
            }
        }

        bitboard.move_down();

        for x in 0..5 {
            for y in 0..5 {
                if x == 0 && y == 4 {
                    assert!(bitboard.is_on(x, y));
                }
                else {
                    assert!(!bitboard.is_on(x, y));
                }
            }
        }

        bitboard.move_down();

        for x in 0..5 {
            for y in 0..5 {
                assert!(!bitboard.is_on(x, y));
            }
        }
    }

    #[test]
    fn can_move_up() {
        let mut bitboard = bitboard::Bitboard::new(5);

        bitboard.toggle(4, 4);

        for x in 0..5 {
            for y in 0..5 {
                if x == 4 && y == 4 {
                    assert!(bitboard.is_on(x, y));
                }
                else {
                    assert!(!bitboard.is_on(x, y));
                }
            }
        }

        for x in 0..5 {
            bitboard.turn_on(x, 0);
        }

        bitboard.move_up();

        for x in 0..5 {
            for y in 0..5 {
                if x == 4 && y == 3 {
                    assert!(bitboard.is_on(x, y));
                }
                else {
                    assert!(!bitboard.is_on(x, y));
                }
            }
        }

        bitboard.move_up();

        for x in 0..5 {
            for y in 0..5 {
                if x == 4 && y == 2 {
                    assert!(bitboard.is_on(x, y));
                }
                else {
                    assert!(!bitboard.is_on(x, y));
                }
            }
        }

        bitboard.move_up();

        for x in 0..5 {
            for y in 0..5 {
                if x == 4 && y == 1 {
                    assert!(bitboard.is_on(x, y));
                }
                else {
                    assert!(!bitboard.is_on(x, y));
                }
            }
        }

        bitboard.move_up();

        for x in 0..5 {
            for y in 0..5 {
                if x == 4 && y == 0 {
                    assert!(bitboard.is_on(x, y));
                }
                else {
                    assert!(!bitboard.is_on(x, y));
                }
            }
        }

        bitboard.move_up();

        for x in 0..5 {
            for y in 0..5 {
                assert!(!bitboard.is_on(x, y));
            }
        }
    }


    #[test]
    fn can_or_bitboards() {
        let mut bitboard = Bitboard::new(3);
        bitboard.turn_on(2, 0);
        bitboard.turn_on(1, 1);
        bitboard.turn_on(0, 2);
        bitboard.turn_on(2, 2);

        let mut other_bitboard = Bitboard::new(3);
        other_bitboard.turn_on(1, 0);
        other_bitboard.turn_on(0, 1);
        other_bitboard.turn_on(2, 1);
        other_bitboard.turn_on(1, 2);

        bitboard.or(&other_bitboard);

        let mut expected_bitboard = Bitboard::new(3);
        for x in 0..3 {
            for y in 0..3 {
                expected_bitboard.turn_on(x, y);
            }
        }
        expected_bitboard.turn_off(0, 0);

        assert_eq!(bitboard, expected_bitboard);
    }

    #[test]
    fn can_toggle_each_and_every_bit() {
        let mut rng = rand::rng();

        for _ in 0..10 {
            let width: u32 = rng.random_range(15..46);
            let height: u32 = rng.random_range(10..31);

            let mut bitboard = Bitboard::new(width);

            for x in 0..width {
                for y in 0..height {
                    bitboard.toggle(x, y);
                    assert!(bitboard.is_on(x, y));
                    bitboard.toggle(x, y);
                    assert!(!(bitboard.is_on(x, y)));
                    bitboard.turn_on(x, y);
                    assert!(bitboard.is_on(x, y));
                    bitboard.turn_on(x, y);
                    assert!(bitboard.is_on(x, y));
                    bitboard.turn_off(x, y);
                    assert!(!bitboard.is_on(x, y));
                    bitboard.turn_off(x, y);
                    assert!(!bitboard.is_on(x, y));
                    bitboard.toggle(x, y);
                    assert!(bitboard.is_on(x, y));
                }
            }

            for x in 0..width {
                for y in 0..height {
                    bitboard.turn_on(x, y);
                }
            }

            for x in 0..width {
                for y in 0..height {
                    bitboard.toggle(x, y);
                    assert!(!bitboard.is_on(x, y));
                    for other_x in 0..width {
                        for other_y in 0..height {
                            if other_x == x && other_y == y {
                                continue
                            }

                            assert!(bitboard.is_on(other_x, other_y));
                        }
                    }

                    bitboard.toggle(x, y);
                    assert!(bitboard.is_on(x, y));
                    for other_x in 0..width {
                        for other_y in 0..height {
                            if other_x == x && other_y == y {
                                continue
                            }

                            assert!(bitboard.is_on(other_x, other_y));
                        }
                    }

                    bitboard.turn_off(x, y);
                    assert!(!bitboard.is_on(x, y));
                    for other_x in 0..width {
                        for other_y in 0..height {
                            if other_x == x && other_y == y {
                                continue
                            }

                            assert!(bitboard.is_on(other_x, other_y));
                        }
                    }

                    bitboard.turn_off(x, y);
                    assert!(!bitboard.is_on(x, y));
                    for other_x in 0..width {
                        for other_y in 0..height {
                            if other_x == x && other_y == y {
                                continue
                            }

                            assert!(bitboard.is_on(other_x, other_y));
                        }
                    }

                    bitboard.turn_on(x, y);
                    assert!(bitboard.is_on(x, y));
                    for other_x in 0..width {
                        for other_y in 0..height {
                            if other_x == x && other_y == y {
                                continue
                            }

                            assert!(bitboard.is_on(other_x, other_y));
                        }
                    }

                    bitboard.turn_on(x, y);
                    assert!(bitboard.is_on(x, y));
                    for other_x in 0..width {
                        for other_y in 0..height {
                            if other_x == x && other_y == y {
                                continue
                            }

                            assert!(bitboard.is_on(other_x, other_y));
                        }
                    }

                }
            }
        }
    }
}
