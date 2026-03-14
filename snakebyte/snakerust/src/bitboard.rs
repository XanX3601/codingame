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

    pub fn new(width: u32) -> Bitboard {
        Bitboard {
            bits: [0; 22],
            width: width,
        }
    }

    pub fn is_on(&self, x: u32, y: u32) -> bool {
        let bit_index = (y * self.width + x) as usize;
        (self.bits[bit_index / 64] >> (bit_index % 64)) & 1 == 1
    }

    pub fn toggle(&mut self, x: u32, y: u32) {
        let bit_index = (y * self.width + x) as usize;
        self.bits[bit_index / 64] ^= 1 << (bit_index % 64);
    }

    pub fn turn_on(&mut self, x: u32, y: u32) {
        let bit_index = (y * self.width + x) as usize;
        self.bits[bit_index / 64] |= 1 << (bit_index % 64);
    }
}

#[cfg(test)]
mod test {
    use rand::RngExt;
    use crate::bitboard;

    use super::*;

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
    fn can_create_one() {
        let _ = Bitboard::new(15);
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
                    bitboard.toggle(x, y);
                    assert!(!bitboard.is_on(x, y));
                }
            }
        }
    }
}
