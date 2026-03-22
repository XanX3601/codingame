#[derive(Clone, Copy, Debug, PartialEq)]
pub struct Bitboard {
    bits: [u64; Bitboard::BITS_LEN],
}

impl Bitboard {
    const BITS_LEN: usize = 22;

    pub fn clear(&mut self) {
        self.bits = [0; Bitboard::BITS_LEN];
    }

    pub fn coord_to_index(x: u16, y: u16, width: u16) -> usize {
        return (y * width + x) as usize;
    }

    pub fn does_collide(&self, bitboard: &Bitboard) -> bool {
        for i in 0..Bitboard::BITS_LEN {
            if self.bits[i] & bitboard.bits[i] > 0{
                return true;
            }
        }
        
        false
    }

    pub fn is_not_empty(&self) -> bool {
        self.bits.iter().any(|&bit| bit != 0)
    }

    pub fn is_on(&self, bit_index: usize) -> bool {
        (self.bits[bit_index / 64] >> (bit_index % 64)) & 1 == 1
    }

    pub fn move_down_in_place(&mut self, width: u16) {
        let inv_width = 64 - width;

        for i in (0..Bitboard::BITS_LEN).rev() {
            self.bits[i] <<= width;

            if i > 0 {
                self.bits[i] |= self.bits[i - 1] >> inv_width;
            }
        }
    }

    pub fn move_up(&self, width: u16) -> Bitboard {
        let mut bits: [u64; Bitboard::BITS_LEN] = [0; Bitboard::BITS_LEN];

        let inv_width = 64 - width;

        for i in 0..22 {
            bits[i] |= self.bits[i] >> width;

            if i + 1 < self.bits.len() {
                bits[i] |= self.bits[i + 1] << inv_width;
            }
        }

        Bitboard {
            bits: bits,
        }
    }

    pub fn move_up_in_place(&mut self, width: u16) {
        let inv_width = 64 - width;

        for i in 0..Bitboard::BITS_LEN {
            self.bits[i] >>= width;

            if i + 1 < self.bits.len() {
                self.bits[i] |= self.bits[i + 1] << inv_width;
            }
        }
    }

    pub fn new() -> Bitboard {
        Bitboard {
            bits: [0; Bitboard::BITS_LEN],
        }
    }

    pub fn or(&self, bitboard: &Bitboard) -> Bitboard {
        let mut bits: [u64; Bitboard::BITS_LEN] = [0; Bitboard::BITS_LEN];
        
        for i in 0..Bitboard::BITS_LEN {
            bits[i] |= self.bits[i] | bitboard.bits[i];
        }

        Bitboard {
            bits: bits,
        }
    }

    pub fn or_inplace(&mut self, bitboard: &Bitboard) {
        for i in 0..Bitboard::BITS_LEN {
            self.bits[i] |= bitboard.bits[i];
        }
    }

    pub fn turn_on(&mut self, bit_index: usize) {
        self.bits[bit_index / 64] |= 1 << (bit_index % 64);
    }

    pub fn turn_off(&mut self, bit_index: usize) {
        self.bits[bit_index / 64] &= !(1 << (bit_index % 64));
    }

    pub fn xor(& self, bitboard: &Bitboard) -> Bitboard{
        let mut bits: [u64; Bitboard::BITS_LEN] = [0; Bitboard::BITS_LEN];

        for i in 0..22 {
            bits[i] = self.bits[i] ^ bitboard.bits[i];
        }

        Bitboard {
            bits: bits,
        }
    }
    pub fn xor_inplace(&mut self, bitboard: &Bitboard) {
        for i in 0..Bitboard::BITS_LEN {
            self.bits[i] ^= bitboard.bits[i];
        }
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

        let width: u16 = rng.random_range(15..46);
        let height: u16 = rng.random_range(10..31);

        let mut bitboard = Bitboard::new();

        for _ in 0..10 {
            let x: u16 = rng.random_range(0..width);
            let y: u16 = rng.random_range(0..height);

            bitboard.turn_on(Bitboard::coord_to_index(x, y, width));
        }

        bitboard.clear();

        for x in 0..width {
            for y in 0..height {
                assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
            }
        }
    }

    #[test]
    fn can_collide_with() {
        let mut bitboard = Bitboard::new();
        bitboard.turn_on(Bitboard::coord_to_index(0, 0, 3));
        bitboard.turn_on(Bitboard::coord_to_index(2, 0, 3));
        bitboard.turn_on(Bitboard::coord_to_index(1, 1, 3));
        bitboard.turn_on(Bitboard::coord_to_index(0, 2, 3));
        bitboard.turn_on(Bitboard::coord_to_index(2, 2, 3));

        let mut other_bitboard = Bitboard::new();
        other_bitboard.turn_on(Bitboard::coord_to_index(1, 0, 3));
        other_bitboard.turn_on(Bitboard::coord_to_index(0, 1, 3));
        other_bitboard.turn_on(Bitboard::coord_to_index(2, 1, 3));
        other_bitboard.turn_on(Bitboard::coord_to_index(1, 2, 3));

        assert!(!bitboard.does_collide(&other_bitboard));
        assert!(!other_bitboard.does_collide(&bitboard));

        assert!(bitboard.does_collide(&bitboard));
        assert!(other_bitboard.does_collide(&other_bitboard));

        bitboard.turn_on(Bitboard::coord_to_index(1, 0, 3));

        assert!(bitboard.does_collide(&other_bitboard));
        assert!(other_bitboard.does_collide(&bitboard));
    }

    #[test]
    fn can_create_one() {
        let _ = Bitboard::new();
    }

    #[test]
    fn can_move_down() {
        let width = 5;

        let mut bitboard = bitboard::Bitboard::new();

        bitboard.turn_on(Bitboard::coord_to_index(0, 0, width));

        for x in 0..width {
            for y in 0..width {
                if x == 0 && y == 0 {
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
                else {
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
            }
        }

        for x in 0..5 {
            bitboard.turn_on(Bitboard::coord_to_index(x, 4, width));
        }

        bitboard.move_down_in_place(width);

        for x in 0..5 {
            for y in 0..5 {
                if x == 0 && y == 1 {
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
                else {
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
            }
        }

        bitboard.move_down_in_place(width);

        for x in 0..5 {
            for y in 0..5 {
                if x == 0 && y == 2 {
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
                else {
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
            }
        }

        bitboard.move_down_in_place(width);

        for x in 0..5 {
            for y in 0..5 {
                if x == 0 && y == 3 {
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
                else {
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
            }
        }

        bitboard.move_down_in_place(width);

        for x in 0..5 {
            for y in 0..5 {
                if x == 0 && y == 4 {
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
                else {
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
            }
        }

        bitboard.move_down_in_place(width);

        for x in 0..5 {
            for y in 0..5 {
                assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
            }
        }
    }

    #[test]
    fn can_move_up() {
        let width = 5;
        let mut bitboard = bitboard::Bitboard::new();

        bitboard.turn_on(Bitboard::coord_to_index(4, 4, width));

        for x in 0..5 {
            for y in 0..5 {
                if x == 4 && y == 4 {
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
                else {
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
            }
        }

        for x in 0..5 {
            bitboard.turn_on(Bitboard::coord_to_index(x, 0, width));
        }

        let not_in_place_bitboard = bitboard.move_up(width);
        bitboard.move_up_in_place(width);

        for x in 0..5 {
            for y in 0..5 {
                if x == 4 && y == 3 {
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    assert!(not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
                else {
                    assert!(!not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
            }
        }

        let not_in_place_bitboard = bitboard.move_up(width);
        bitboard.move_up_in_place(width);

        for x in 0..5 {
            for y in 0..5 {
                if x == 4 && y == 2 {
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    assert!(not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
                else {
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    assert!(!not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
            }
        }

        let not_in_place_bitboard = bitboard.move_up(width);
        bitboard.move_up_in_place(width);

        for x in 0..5 {
            for y in 0..5 {
                if x == 4 && y == 1 {
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    assert!(not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
                else {
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    assert!(!not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
            }
        }

        let not_in_place_bitboard = bitboard.move_up(width);
        bitboard.move_up_in_place(width);

        for x in 0..5 {
            for y in 0..5 {
                if x == 4 && y == 0 {
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    assert!(not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
                else {
                    assert!(!not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
            }
        }

        let not_in_place_bitboard = bitboard.move_up(width);
        bitboard.move_up_in_place(width);

        for x in 0..5 {
            for y in 0..5 {
                assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                assert!(!not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
            }
        }
    }


    #[test]
    fn can_or_bitboards() {
        let width = 3;

        let mut bitboard = Bitboard::new();
        bitboard.turn_on(Bitboard::coord_to_index(2, 0, width));
        bitboard.turn_on(Bitboard::coord_to_index(1, 1, width));
        bitboard.turn_on(Bitboard::coord_to_index(0, 2, width));
        bitboard.turn_on(Bitboard::coord_to_index(2, 2, width));

        let mut other_bitboard = Bitboard::new();
        other_bitboard.turn_on(Bitboard::coord_to_index(1, 0, width));
        other_bitboard.turn_on(Bitboard::coord_to_index(0, 1, width));
        other_bitboard.turn_on(Bitboard::coord_to_index(2, 1, width));
        other_bitboard.turn_on(Bitboard::coord_to_index(1, 2, width));

        let not_in_place_bitboard = bitboard.or(&other_bitboard);

        bitboard.or_inplace(&other_bitboard);

        let mut expected_bitboard = Bitboard::new();
        for x in 0..3 {
            for y in 0..3 {
                expected_bitboard.turn_on(Bitboard::coord_to_index(x, y, width));
            }
        }
        expected_bitboard.turn_off(Bitboard::coord_to_index(0, 0, width));

        assert_eq!(bitboard, expected_bitboard);
        assert_eq!(not_in_place_bitboard, expected_bitboard);
    }

    #[test]
    fn can_toggle_each_and_every_bit() {
        let mut rng = rand::rng();

        for _ in 0..10 {
            let width: u16= rng.random_range(15..46);
            let height: u16 = rng.random_range(10..31);

            let mut bitboard = Bitboard::new();

            for x in 0..width {
                for y in 0..height {
                    bitboard.turn_on(Bitboard::coord_to_index(x, y, width));
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    bitboard.turn_on(Bitboard::coord_to_index(x, y, width));
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    bitboard.turn_off(Bitboard::coord_to_index(x, y, width));
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    bitboard.turn_off(Bitboard::coord_to_index(x, y, width));
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
            }

            for x in 0..width {
                for y in 0..height {
                    bitboard.turn_on(Bitboard::coord_to_index(x, y, width));
                }
            }

            for x in 0..width {
                for y in 0..height {
                    bitboard.turn_off(Bitboard::coord_to_index(x, y, width));
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    for other_x in 0..width {
                        for other_y in 0..height {
                            if other_x == x && other_y == y {
                                continue
                            }

                            assert!(bitboard.is_on(Bitboard::coord_to_index(other_x, other_y, width)));
                        }
                    }

                    bitboard.turn_on(Bitboard::coord_to_index(x, y, width));
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    for other_x in 0..width {
                        for other_y in 0..height {
                            if other_x == x && other_y == y {
                                continue
                            }

                            assert!(bitboard.is_on(Bitboard::coord_to_index(other_x, other_y, width)));
                        }
                    }

                    bitboard.turn_off(Bitboard::coord_to_index(x, y, width));
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    for other_x in 0..width {
                        for other_y in 0..height {
                            if other_x == x && other_y == y {
                                continue
                            }

                            assert!(bitboard.is_on(Bitboard::coord_to_index(other_x, other_y, width)));
                        }
                    }

                    bitboard.turn_off(Bitboard::coord_to_index(x, y, width));
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    for other_x in 0..width {
                        for other_y in 0..height {
                            if other_x == x && other_y == y {
                                continue
                            }

                            assert!(bitboard.is_on(Bitboard::coord_to_index(other_x, other_y, width)));
                        }
                    }

                    bitboard.turn_on(Bitboard::coord_to_index(x, y, width));
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    for other_x in 0..width {
                        for other_y in 0..height {
                            if other_x == x && other_y == y {
                                continue
                            }

                            assert!(bitboard.is_on(Bitboard::coord_to_index(other_x, other_y, width)));
                        }
                    }

                    bitboard.turn_on(Bitboard::coord_to_index(x, y, width));
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    for other_x in 0..width {
                        for other_y in 0..height {
                            if other_x == x && other_y == y {
                                continue
                            }

                            assert!(bitboard.is_on(Bitboard::coord_to_index(other_x, other_y, width)));
                        }
                    }

                }
            }
        }
    }

    #[test]
    fn can_xor_bitboards() {
        let width = 3;

        let mut bitboard = Bitboard::new();
        bitboard.turn_on(Bitboard::coord_to_index(2, 0, width));
        bitboard.turn_on(Bitboard::coord_to_index(1, 1, width));
        bitboard.turn_on(Bitboard::coord_to_index(0, 2, width));
        bitboard.turn_on(Bitboard::coord_to_index(2, 2, width));

        let mut other_bitboard = Bitboard::new();
        other_bitboard.turn_on(Bitboard::coord_to_index(2, 0, width));
        other_bitboard.turn_on(Bitboard::coord_to_index(0, 0, width));

        let not_in_place_bitboard = bitboard.xor(&other_bitboard);
        bitboard.xor_inplace(&other_bitboard);

        let mut expected_bitboard = Bitboard::new();
        expected_bitboard.turn_on(Bitboard::coord_to_index(0, 0, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(1, 1, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(0, 2, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(2, 2, width));

        assert_eq!(bitboard, expected_bitboard);
        assert_eq!(not_in_place_bitboard, expected_bitboard);
    }
}
