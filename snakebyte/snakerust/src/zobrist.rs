pub struct ZobristTable {
    power_sources: [u64; 1350],
    snake_bodies: [[u64; 1350]; 16],
}

impl ZobristTable {
    pub fn get_pwoer_source_hash(&self, cell_index: usize) -> u64 {
        self.power_sources[cell_index]
    }

    pub fn get_snake_body_hash(&self, snake_id: u8, cell_index: usize) -> u64 {
        self.snake_bodies[snake_id as usize][cell_index]
    }

    pub fn new() -> Self {
        let mut seed = 0xDEADC0DE; 
        let mut next_u64 = || { // Xorshift64
            seed ^= seed << 13; seed ^= seed >> 7; seed ^= seed << 17;
            seed
        };

        let mut snake_bodies = [[0; 1350]; 16];
        for snake_id in 0..16 {
            for cell_index in 0..1350 {
                snake_bodies[snake_id][cell_index] = next_u64();
            }
        }

        let mut power_sources = [0; 1350];
        for cell_index in 0..1350 {
            power_sources[cell_index] = next_u64();
        }

        Self {
            power_sources: power_sources,
            snake_bodies: snake_bodies,
        }
    }
}

#[cfg(test)]
mod test {
    use pretty_assertions::assert_eq;

    use crate::bitboard;

    use super::*;

    #[test]
    fn can_create_zobrist_table() {
        ZobristTable::new();
    }

    #[test]
    fn can_use_zobrist_table() {
        let zobrist_table = ZobristTable::new();

        let mut snake_hash = 0;
        snake_hash ^= zobrist_table.get_snake_body_hash(0, bitboard::Bitboard::coord_to_index(0, 0, 5));
        snake_hash ^= zobrist_table.get_snake_body_hash(0, bitboard::Bitboard::coord_to_index(0, 1, 5));
        snake_hash ^= zobrist_table.get_snake_body_hash(0, bitboard::Bitboard::coord_to_index(0, 2, 5));
        snake_hash ^= zobrist_table.get_snake_body_hash(0, bitboard::Bitboard::coord_to_index(1, 2, 5));

        let mut other_snake_hash = 0;
        other_snake_hash ^= zobrist_table.get_snake_body_hash(0, bitboard::Bitboard::coord_to_index(0, 0, 5));
        other_snake_hash ^= zobrist_table.get_snake_body_hash(0, bitboard::Bitboard::coord_to_index(0, 1, 5));
        other_snake_hash ^= zobrist_table.get_snake_body_hash(0, bitboard::Bitboard::coord_to_index(0, 2, 5));
        other_snake_hash ^= zobrist_table.get_snake_body_hash(0, bitboard::Bitboard::coord_to_index(1, 2, 5));

        assert_eq!(snake_hash, other_snake_hash);

        let mut other_snake_hash = 0;
        other_snake_hash ^= zobrist_table.get_snake_body_hash(1, bitboard::Bitboard::coord_to_index(0, 0, 5));
        other_snake_hash ^= zobrist_table.get_snake_body_hash(1, bitboard::Bitboard::coord_to_index(0, 1, 5));
        other_snake_hash ^= zobrist_table.get_snake_body_hash(1, bitboard::Bitboard::coord_to_index(0, 2, 5));
        other_snake_hash ^= zobrist_table.get_snake_body_hash(1, bitboard::Bitboard::coord_to_index(1, 2, 5));

        assert_ne!(snake_hash, other_snake_hash);

        let mut power_source_hash = 0;
        power_source_hash ^= zobrist_table.get_pwoer_source_hash(bitboard::Bitboard::coord_to_index(0, 0, 5));

        let mut other_power_source_hash = 0;
        other_power_source_hash ^= zobrist_table.get_pwoer_source_hash(bitboard::Bitboard::coord_to_index(0, 0, 5));

        assert_eq!(power_source_hash, other_power_source_hash);

        power_source_hash ^= zobrist_table.get_pwoer_source_hash(bitboard::Bitboard::coord_to_index(0, 1, 5));

        assert_ne!(power_source_hash, other_power_source_hash);

        power_source_hash ^= zobrist_table.get_pwoer_source_hash(bitboard::Bitboard::coord_to_index(0, 1, 5));

        assert_eq!(power_source_hash, other_power_source_hash);

        power_source_hash ^= zobrist_table.get_pwoer_source_hash(bitboard::Bitboard::coord_to_index(0, 1, 5));

        assert_ne!(power_source_hash ^ snake_hash, other_power_source_hash ^ snake_hash);
    }
}
