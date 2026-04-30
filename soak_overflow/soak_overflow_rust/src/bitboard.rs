use crate::parameters::{Parameters};

#[derive(Clone, Copy, Debug, PartialEq)]
pub struct Bitboard {
    bits: [u64; ((Parameters::WIDTH_MAX * Parameters::HEIGHT_MAX) as usize / 64) + 1]
}

impl Bitboard {
    const BIT_COUNT: usize = 64;

    pub fn is_on(&self, cell_index: usize) -> bool {
        self.bits[cell_index / Self::BIT_COUNT] & 1 << (cell_index % Self::BIT_COUNT) > 0
    }

    pub fn new() -> Self {
        Self {
            bits: [0; (Parameters::WIDTH_MAX * Parameters::HEIGHT_MAX / 64) + 1]
        }
    }

    pub fn or(&self, other_bitboard: &Self) -> Self {
        let mut new_bitboard = *self;
        new_bitboard.or_inplace(other_bitboard);
        new_bitboard
    }

    pub fn or_inplace(&mut self, other_bitboard: &Self) {
        for bit_index in 0..self.bits.len() {
            self.bits[bit_index] |= other_bitboard.bits[bit_index];
        }
    }

    pub fn turn_off(&mut self, cell_index: usize) {
        self.bits[cell_index / Self::BIT_COUNT] &= !(1 << (cell_index % Self::BIT_COUNT));
    }

    pub fn turn_on(&mut self, cell_index: usize) {
        self.bits[cell_index / Self::BIT_COUNT] |= 1 << (cell_index % Self::BIT_COUNT);
    }
}
