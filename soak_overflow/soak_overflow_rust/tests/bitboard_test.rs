use soak_overflow_rust::bitboard::{self, Bitboard};
use soak_overflow_rust::parameters::{Parameters};

#[test]
fn can_create_bitboard() {
    Bitboard::new();
}

#[test]
fn can_turn_on_and_off_cells() {
    let mut bitboard = Bitboard::new();

    for x in 0..Parameters::WIDTH_MAX {
        for y in 0..Parameters::HEIGHT_MAX {
            let x = x as u8;
            let y = y as u8;

            let cell_index = Parameters::coord_to_index(x, y, Parameters::WIDTH_MAX as u8);
            assert!(!bitboard.is_on(cell_index));
            bitboard.turn_on(cell_index);
            assert!(bitboard.is_on(cell_index));
        }
    }

    for x in 0..Parameters::WIDTH_MAX {
        for y in 0..Parameters::HEIGHT_MAX {
            let x = x as u8;
            let y = y as u8;

            let cell_index = Parameters::coord_to_index(x, y, Parameters::WIDTH_MAX as u8);
            assert!(bitboard.is_on(cell_index));
            bitboard.turn_off(cell_index);
            assert!(!bitboard.is_on(cell_index));
        }
    }
}

#[test]
fn can_or_bitboards() {
    let mut bitboard_1 = Bitboard::new();
    let mut bitboard_2 = Bitboard::new();

    bitboard_1.turn_on(5);
    bitboard_2.turn_on(2);

    let or_bitboard = bitboard_1.or(&bitboard_2);

    for x in 0..Parameters::WIDTH_MAX {
        for y in 0..Parameters::HEIGHT_MAX {
            let x = x as u8;
            let y = y as u8;

            let cell_index = Parameters::coord_to_index(x, y, 10);

            if cell_index == 5 || cell_index == 2 {
                assert!(or_bitboard.is_on(cell_index));
            }
            else {
                assert!(!or_bitboard.is_on(cell_index));
            }
        }
    }

    bitboard_1.or_inplace(&bitboard_2);

    assert_eq!(bitboard_1, or_bitboard);
}

#[test]
fn new_bitboard_is_empty() {
    let bitboard = Bitboard::new();

    for x in 0..Parameters::WIDTH_MAX {
        for y in 0..Parameters::HEIGHT_MAX {
            let x = x as u8;
            let y = y as u8;

            assert!(!bitboard.is_on(Parameters::coord_to_index(x, y, Parameters::WIDTH_MAX as u8)));
        }
    }
}
