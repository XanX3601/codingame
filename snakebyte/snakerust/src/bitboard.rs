#[derive(Clone, Copy, Debug, PartialEq)]
pub struct Bitboard {
    bits: [u64; Bitboard::BITS_LEN],
}

impl Bitboard {
    const BITS_LEN: usize = 22;

    pub fn and(&self, bitboard: &Self) -> Self {
        let mut bits = [0; Self::BITS_LEN];

        for i in 0..Self::BITS_LEN {
            bits[i] = self.bits[i] & bitboard.bits[i];
        }

        Self {
            bits: bits
        }
    }

    pub fn and_not_inplace(&mut self, bitboard: &Self) {
        for i in 0..Self::BITS_LEN {
            self.bits[i] &= !bitboard.bits[i];
        }
    }

    pub fn clear(&mut self) {
        self.bits = [0; Bitboard::BITS_LEN];
    }

    pub fn coord_to_index(x: u16, y: u16, width: u16) -> usize {
        return (y * width + x) as usize;
    }

    pub fn count_collide(&self, bitboard: &Self) -> u32 {
        let mut collide_count = 0;
        for i in 0..Self::BITS_LEN {
            collide_count += (self.bits[i] & bitboard.bits[i]).count_ones();
        }
        collide_count
    }

    pub fn count_ones(&self) -> u32 {
        let mut one_count = 0;
        for i in 0..Self::BITS_LEN {
            one_count += self.bits[i].count_ones();
        }
        one_count
    }

    pub fn does_collide(&self, bitboard: &Bitboard) -> bool {
        for i in 0..Self::BITS_LEN {
            if self.bits[i] & bitboard.bits[i] > 0{
                return true;
            }
        }
        
        false
    }

    pub fn expand_in_place(&mut self, width: u16, bitboard_masks: &BitboardMasks) {
        let copy = self.clone();

        self.or_inplace(&copy.move_down(width));
        self.or_inplace(&copy.move_left(bitboard_masks));
        self.or_inplace(&copy.move_right(bitboard_masks));
        self.or_inplace(&copy.move_up(width));
    }

    pub fn is_not_empty(&self) -> bool {
        self.bits.iter().any(|&bit| bit != 0)
    }

    pub fn is_on(&self, bit_index: usize) -> bool {
        (self.bits[bit_index / 64] >> (bit_index % 64)) & 1 == 1
    }

    pub fn move_down(&self, width: u16) -> Self {
        let mut bits = [0; Self::BITS_LEN];
        let inv_width = 64 - width;

        for i in (0..Bitboard::BITS_LEN).rev() {
            bits[i] = self.bits[i] << width;

            if i > 0 {
                bits[i] |= self.bits[i - 1] >> inv_width;
            }
        }
         
        Self {
            bits: bits
        }
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

    pub fn move_left(&self, masks: &BitboardMasks) -> Bitboard {
        let mut bits = [0; Self::BITS_LEN];

        for i in 0..Self::BITS_LEN {
            bits[i] = (self.bits[i] & masks.not_left_column.bits[i]) >> 1;
            if i + 1 < Self::BITS_LEN {
                bits[i] |= self.bits[i + 1] << 63;
            }
        }

        Self {
            bits: bits
        }
    }

    pub fn move_right(&self, masks: &BitboardMasks) -> Bitboard {
        let mut bits = [0; Self::BITS_LEN];

        for i in 0..Self::BITS_LEN {
            bits[i] = (self.bits[i] & masks.not_right_column.bits[i]) << 1;
            if i > 0 {
                bits[i] |= self.bits[i - 1] >> 63;
            }
        }

        Self {
            bits: bits
        }
    }

    pub fn move_up(&self, width: u16) -> Bitboard {
        let mut bits: [u64; Bitboard::BITS_LEN] = [0; Bitboard::BITS_LEN];

        let inv_width = 64 - width;

        for i in 0..22 {
            bits[i] |= self.bits[i] >> width;

            if i + 1 < Self::BITS_LEN {
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

pub struct BitboardMasks {
    not_left_column: Bitboard,
    not_right_column: Bitboard,
}

impl BitboardMasks {
    pub fn get_not_left_column_masks(&self) -> Bitboard {
        self.not_left_column
    }

    pub fn get_not_right_column_masks(&self) -> Bitboard {
        self.not_right_column
    }

    pub fn new(width: u16, height: u16) -> Self {
        let mut not_left_column = Bitboard::new();
        let mut not_right_column = Bitboard::new();

        for x in 0..width {
            for y in 0..height {
                if x > 0 {
                    not_left_column.turn_on(Bitboard::coord_to_index(x, y, width));
                }
                if x < width - 1 {
                    not_right_column.turn_on(Bitboard::coord_to_index(x, y, width));
                }
            }
        }

        Self {
            not_left_column: not_left_column,
            not_right_column: not_right_column,
        }
    }
}

#[cfg(test)]
mod test {
    use crate::bitboard;

    use super::*;

    use pretty_assertions::assert_eq;
    use rand::RngExt;

    #[test]
    fn can_and() {
        let width = 5;

        let mut bitboard = Bitboard::new();
        bitboard.turn_on(Bitboard::coord_to_index(2, 2, width));
        bitboard.turn_on(Bitboard::coord_to_index(1, 2, width));
        bitboard.turn_on(Bitboard::coord_to_index(3, 2, width));
        bitboard.turn_on(Bitboard::coord_to_index(2, 1, width));
        bitboard.turn_on(Bitboard::coord_to_index(2, 3, width));

        let mut other_bitboard = Bitboard::new();
        other_bitboard.turn_on(Bitboard::coord_to_index(2, 2, width));

        let not_in_place_bitboard = bitboard.and(&other_bitboard);

        let mut expected_bitboard = Bitboard::new();
        expected_bitboard.turn_on(Bitboard::coord_to_index(2, 2, width));

        assert_eq!(not_in_place_bitboard, expected_bitboard);
    }


    #[test]
    fn can_and_not() {
        let width = 5;

        let mut bitboard = Bitboard::new();
        bitboard.turn_on(Bitboard::coord_to_index(2, 2, width));
        bitboard.turn_on(Bitboard::coord_to_index(1, 2, width));
        bitboard.turn_on(Bitboard::coord_to_index(3, 2, width));
        bitboard.turn_on(Bitboard::coord_to_index(2, 1, width));
        bitboard.turn_on(Bitboard::coord_to_index(2, 3, width));

        let mut other_bitboard = Bitboard::new();
        other_bitboard.turn_on(Bitboard::coord_to_index(2, 2, width));

        bitboard.and_not_inplace(&other_bitboard);

        let mut expected_bitboard = Bitboard::new();
        expected_bitboard.turn_on(Bitboard::coord_to_index(1, 2, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(3, 2, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(2, 1, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(2, 3, width));

        assert_eq!(bitboard, expected_bitboard);
    }

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
    fn can_count_collide() {
        let mut bitboard = Bitboard::new();
        bitboard.turn_on(Bitboard::coord_to_index(0, 0, 3));
        bitboard.turn_on(Bitboard::coord_to_index(2, 0, 3));
        bitboard.turn_on(Bitboard::coord_to_index(1, 1, 3));
        bitboard.turn_on(Bitboard::coord_to_index(0, 2, 3));
        bitboard.turn_on(Bitboard::coord_to_index(2, 2, 3));

        assert_eq!(bitboard.count_collide(&bitboard), 5);

        let mut other_bitboard = Bitboard::new();
        other_bitboard.turn_on(Bitboard::coord_to_index(1, 0, 3));
        other_bitboard.turn_on(Bitboard::coord_to_index(0, 1, 3));
        other_bitboard.turn_on(Bitboard::coord_to_index(2, 1, 3));
        other_bitboard.turn_on(Bitboard::coord_to_index(1, 2, 3));

        assert_eq!(other_bitboard.count_collide(&bitboard), 0);
        assert_eq!(other_bitboard.count_collide(&other_bitboard), 4);

        bitboard.turn_on(Bitboard::coord_to_index(0, 1, 3));
        bitboard.turn_on(Bitboard::coord_to_index(2, 1, 3));
        bitboard.turn_on(Bitboard::coord_to_index(1, 2, 3));

        assert_eq!(other_bitboard.count_collide(&bitboard), 3);
        assert_eq!(bitboard.count_collide(&other_bitboard), 3);
    }

    #[test]
    fn can_count_ones() {
        let mut bitboard = Bitboard::new();
        bitboard.turn_on(0);
        bitboard.turn_on(30);
        bitboard.turn_on(500);
        bitboard.turn_on(1);

        assert_eq!(bitboard.count_ones(), 4);
    }

    #[test]
    fn can_create_bitboard() {
        let _ = Bitboard::new();
    }

    #[test]
    fn can_create_bitboard_masks() {
        BitboardMasks::new(26, 14);
    }

    #[test]
    fn can_expand() {
        let width = 5;
        let height = 5;

        let bitboard_masks = BitboardMasks::new(width, height);

        let mut bitboard = Bitboard::new();
        bitboard.turn_on(Bitboard::coord_to_index(2, 2, width));

        bitboard.expand_in_place(width, &bitboard_masks);

        let mut expected_bitboard = Bitboard::new();
        expected_bitboard.turn_on(Bitboard::coord_to_index(2, 2, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(1, 2, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(3, 2, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(2, 1, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(2, 3, width));

        assert_eq!(bitboard, expected_bitboard);

        bitboard.expand_in_place(width, &bitboard_masks);

        let mut expected_bitboard = Bitboard::new();
        expected_bitboard.turn_on(Bitboard::coord_to_index(2, 2, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(1, 2, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(3, 2, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(2, 1, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(2, 3, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(0, 2, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(4, 2, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(2, 0, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(2, 4, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(1, 1, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(3, 1, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(1, 3, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(3, 3, width));

        assert_eq!(bitboard, expected_bitboard);
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

        let not_in_place_bitboard = bitboard.move_down(width);
        bitboard.move_down_in_place(width);

        for x in 0..5 {
            for y in 0..5 {
                if x == 0 && y == 1 {
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    assert!(not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
                else {
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    assert!(!not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
            }
        }

        let not_in_place_bitboard = bitboard.move_down(width);
        bitboard.move_down_in_place(width);

        for x in 0..5 {
            for y in 0..5 {
                if x == 0 && y == 2 {
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    assert!(not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
                else {
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    assert!(!not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
            }
        }

        let not_in_place_bitboard = bitboard.move_down(width);
        bitboard.move_down_in_place(width);

        for x in 0..5 {
            for y in 0..5 {
                if x == 0 && y == 3 {
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    assert!(not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
                else {
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    assert!(!not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
            }
        }

        let not_in_place_bitboard = bitboard.move_down(width);
        bitboard.move_down_in_place(width);

        for x in 0..5 {
            for y in 0..5 {
                if x == 0 && y == 4 {
                    assert!(bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    assert!(not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
                else {
                    assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                    assert!(!not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                }
            }
        }

        let not_in_place_bitboard = bitboard.move_down(width);
        bitboard.move_down_in_place(width);

        for x in 0..5 {
            for y in 0..5 {
                assert!(!bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
                assert!(!not_in_place_bitboard.is_on(Bitboard::coord_to_index(x, y, width)));
            }
        }
    }

    #[test]
    fn can_move_left() {
        let width = 3;
        let height = 4;
        let bitboard_masks = BitboardMasks::new(width, height);

        let mut bitboard = Bitboard::new();
        bitboard.turn_on(Bitboard::coord_to_index(0, 0, width));
        bitboard.turn_on(Bitboard::coord_to_index(2, 1, width));
        bitboard.turn_on(Bitboard::coord_to_index(2, 2, width));
        bitboard.turn_on(Bitboard::coord_to_index(0, 2, width));
        bitboard.turn_on(Bitboard::coord_to_index(0, 3, width));
        bitboard.turn_on(Bitboard::coord_to_index(1, 3, width));
        bitboard.turn_on(Bitboard::coord_to_index(2, 3, width));

        bitboard = bitboard.move_left(&bitboard_masks);

        let mut expected_bitboard = Bitboard::new();
        expected_bitboard.turn_on(Bitboard::coord_to_index(1, 1, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(1, 2, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(0, 3, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(1, 3, width));

        assert_eq!(bitboard, expected_bitboard);
    }

    #[test]
    fn can_move_left_bigger() {
        let width = 40;
        let height = 3;
        let bitboard_masks = BitboardMasks::new(width, height);

        let mut bitboard = Bitboard::new();
        bitboard.turn_on(Bitboard::coord_to_index(39, 0, width));
        bitboard.turn_on(Bitboard::coord_to_index(39, 1, width));
        bitboard.turn_on(Bitboard::coord_to_index(39, 2, width));

        for x in 0..width - 1 {
            bitboard = bitboard.move_left(&bitboard_masks);

            let mut expected_bitboard = Bitboard::new();
            expected_bitboard.turn_on(Bitboard::coord_to_index(width - x - 2, 0, width));
            expected_bitboard.turn_on(Bitboard::coord_to_index(width - x - 2, 1, width));
            expected_bitboard.turn_on(Bitboard::coord_to_index(width - x - 2, 2, width));

            assert_eq!(bitboard, expected_bitboard);
        }
    }

    #[test]
    fn can_move_right() {
        let width = 3;
        let height = 4;
        let bitboard_masks = BitboardMasks::new(width, height);

        let mut bitboard = Bitboard::new();
        bitboard.turn_on(Bitboard::coord_to_index(0, 0, width));
        bitboard.turn_on(Bitboard::coord_to_index(2, 1, width));
        bitboard.turn_on(Bitboard::coord_to_index(2, 2, width));
        bitboard.turn_on(Bitboard::coord_to_index(0, 2, width));
        bitboard.turn_on(Bitboard::coord_to_index(0, 3, width));
        bitboard.turn_on(Bitboard::coord_to_index(1, 3, width));
        bitboard.turn_on(Bitboard::coord_to_index(2, 3, width));

        bitboard = bitboard.move_right(&bitboard_masks);

        let mut expected_bitboard = Bitboard::new();
        expected_bitboard.turn_on(Bitboard::coord_to_index(1, 0, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(1, 2, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(1, 3, width));
        expected_bitboard.turn_on(Bitboard::coord_to_index(2, 3, width));

        assert_eq!(bitboard, expected_bitboard);
    }

    #[test]
    fn can_move_right_bigger() {
        let width = 40;
        let height = 3;
        let bitboard_masks = BitboardMasks::new(width, height);

        let mut bitboard = Bitboard::new();
        bitboard.turn_on(Bitboard::coord_to_index(0, 0, width));
        bitboard.turn_on(Bitboard::coord_to_index(0, 1, width));
        bitboard.turn_on(Bitboard::coord_to_index(0, 2, width));

        for x in 0..width - 1 {
            bitboard = bitboard.move_right(&bitboard_masks);

            let mut expected_bitboard = Bitboard::new();
            expected_bitboard.turn_on(Bitboard::coord_to_index(x + 1, 0, width));
            expected_bitboard.turn_on(Bitboard::coord_to_index(x + 1, 1, width));
            expected_bitboard.turn_on(Bitboard::coord_to_index(x + 1, 2, width));

            assert_eq!(bitboard, expected_bitboard);
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
