use crate::agent_data::AgentData;
use crate::bitboard::Bitboard;

pub struct Parameters {
    agent_count: usize,
    agent_datas: [Option<AgentData>; Parameters::AGENT_COUNT_MAX],
    cover_bitboard: Bitboard,
    empty_cell_bitboard: Bitboard,
    height: u8,
    high_cover_bitboard: Bitboard,
    low_cover_bitboard: Bitboard,
    my_id: u8,
    width: u8,
}

impl Parameters {
    pub const AGENT_COUNT_MIN: u8 = 6;
    pub const AGENT_COUNT_MAX: usize = 10;
    pub const HEIGHT_MAX: usize = 10;
    pub const HEIGHT_MIN: u8 = 6;
    pub const WIDTH_MAX: usize = 20;
    pub const WIDTH_MIN: u8 = 12;

    pub fn coord_to_index(x: u8, y: u8, width: u8) -> usize {
        (y * width + x) as usize
    }

    pub fn get_agent_count(&self) -> usize {
        self.agent_count
    }

    pub fn get_agent_data(&self, agent_id: usize) -> &Option<AgentData> {
        &self.agent_datas[agent_id - 1]
    }

    pub fn get_cover_bitboard(&self) -> &Bitboard {
        &self.cover_bitboard
    }

    pub fn get_empty_cell_bitboard(&self) -> &Bitboard {
        &self.empty_cell_bitboard
    }

    pub fn get_height(&self) -> u8 {
        self.height
    }

    pub fn get_high_cover_bitboard(&self) -> &Bitboard {
        &self.high_cover_bitboard
    }

    pub fn get_low_cover_bitboard(&self) -> &Bitboard {
        &self.low_cover_bitboard
    }

    pub fn get_my_id(&self) -> u8 {
        self.my_id
    }

    pub fn get_width(&self) -> u8 {
        self.width
    }

    pub fn from_buffer<R: std::io::BufRead>(mut reader: R) -> Self {
        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let my_id = input_line.trim().parse::<u8>().unwrap();

        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let agent_count = input_line.trim().parse::<usize>().unwrap();

        let mut agent_datas = [None; Self::AGENT_COUNT_MAX];
        for agent_index in 0..agent_count {
            let mut input_line = String::new();
            reader.read_line(&mut input_line).unwrap();
            let mut agent_data_iter = input_line
                .trim()
                .split_whitespace()
                .into_iter();

            agent_data_iter.next();

            let player_id = agent_data_iter.next().unwrap().parse::<u8>().unwrap();
            let shoot_cooldown = agent_data_iter.next().unwrap().parse::<u8>().unwrap();
            let optimal_range = agent_data_iter.next().unwrap().parse::<u8>().unwrap();
            let soaking_power = agent_data_iter.next().unwrap().parse::<u8>().unwrap();
            let splash_bomb_count = agent_data_iter.next().unwrap().parse::<u8>().unwrap();

            agent_datas[agent_index] = Some(AgentData::new(
                optimal_range,
                player_id,
                shoot_cooldown,
                soaking_power,
                splash_bomb_count
            ));
        }

        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let mut dimension_iter = input_line
            .trim()
            .split_whitespace()
            .into_iter();

        let width = dimension_iter.next().unwrap().parse::<u8>().unwrap();
        let height = dimension_iter.next().unwrap().parse::<u8>().unwrap();

        let mut empty_cell_bitboard = Bitboard::new();
        let mut high_cover_bitboard = Bitboard::new();
        let mut low_cover_bitboard = Bitboard::new();

        for _ in 0..height {
            let mut input_line = String::new();
            reader.read_line(&mut input_line).unwrap();
            let mut cell_iter = input_line
                .trim()
                .split_whitespace()
                .into_iter();

            for _ in 0..width {
                let x = cell_iter.next().unwrap().parse::<u8>().unwrap();
                let y = cell_iter.next().unwrap().parse::<u8>().unwrap();
                
                let cell_index = Self::coord_to_index(x, y, width);

                let cell_type = cell_iter.next().unwrap().parse::<u8>().unwrap();

                match cell_type {
                    0 => empty_cell_bitboard.turn_on(cell_index),
                    1 => low_cover_bitboard.turn_on(cell_index),
                    2 => high_cover_bitboard.turn_on(cell_index),
                    other => panic!("cell type cannot be {other}"),
                }
            }
        }

        let cover_bitboard = low_cover_bitboard.or(&high_cover_bitboard);

        Self {
            agent_count: agent_count,
            agent_datas: agent_datas,
            cover_bitboard: cover_bitboard,
            empty_cell_bitboard: empty_cell_bitboard,
            height: height,
            high_cover_bitboard: high_cover_bitboard,
            low_cover_bitboard: low_cover_bitboard,
            my_id: my_id,
            width: width,
        }
    }
}
