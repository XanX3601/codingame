use crate::{agent::Agent, bitboard::Bitboard, direction::Direction, parameters::Parameters};

pub struct State {
    agent_bitboard: Bitboard,
    agents: [Option<Agent>; Parameters::AGENT_COUNT_MAX],
}

impl State {
    pub fn from_buffer<R: std::io::BufRead>(mut reader: R, width: u8) -> Self {
        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let agent_count = input_line.trim().parse::<u8>().unwrap();

        let mut agent_bitboard = Bitboard::new();
        let mut agents = [None; Parameters::AGENT_COUNT_MAX];
        for _ in 0..agent_count {
            let mut input_line = String::new();
            reader.read_line(&mut input_line).unwrap();
            println!("{}", input_line);
            let mut agent_iter = input_line
                .trim()
                .split_whitespace()
                .into_iter();


            let agent_id = agent_iter.next().unwrap().parse::<usize>().unwrap();
            let x = agent_iter.next().unwrap().parse::<u8>().unwrap();
            let y = agent_iter.next().unwrap().parse::<u8>().unwrap();
            let shoot_cooldown = agent_iter.next().unwrap().parse::<u8>().unwrap();
            let splash_bomb_count = agent_iter.next().unwrap().parse::<u8>().unwrap();
            let wetness = agent_iter.next().unwrap().parse::<u8>().unwrap();

            agent_bitboard.turn_on(Parameters::coord_to_index(x, y, width));
            agents[agent_id - 1] = Some(Agent::new(x, y, shoot_cooldown, splash_bomb_count, wetness));
        }

        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();

        Self {
            agent_bitboard: agent_bitboard,
            agents: agents
        }
    }

    pub fn get_agent(&self, agent_id: usize) -> &Option<Agent> {
        &self.agents[agent_id - 1]
    }

    pub fn get_agent_bitboard(&self) -> &Bitboard {
        &self.agent_bitboard
    }
}

pub struct Simulator<'a> {
    agent_indexes_queue: std::collections::VecDeque<usize>,
    moves_applied: std::collections::HashMap<(u8, u8), (usize, u8, u8)>,
    parameters: &'a Parameters
}

impl<'a> Simulator<'a> {
    pub fn apply_moves_in_place(&mut self, state: &mut State, moves: &[Option<Direction>]) {
        let loop_count = 0;
        let previous_agent_id = -1;
        while !self.agent_indexes_queue.is_empty() {
            let agent_index = self.agent_indexes_queue.pop_front().unwrap();

            if let Some(agent) = state.agents[agent_index].as_mut() && let Some(direction) = moves[agent_index] {
                let (next_x, next_y) = direction.apply_to(
                    agent.get_x(), agent.get_y(),
                    self.parameters.get_width(), self.parameters.get_height()
                );
                let next_cell_index = Parameters::coord_to_index(next_x, next_y, self.parameters.get_width());

                // if the agent try to go to a wall, do nothing, it cannot go
                // there
                if self.parameters.get_cover_bitboard().is_on(next_cell_index) {
                    continue;
                }

                // if at some point, another agent tried to go into next cell
                // then stop and revert the other agent move
                if let Some((other_agent_index, previous_x, previous_y)) = self.moves_applied.get(&(next_x, next_y)) {
                    
                }

                if state.agent_bitboard.is_on(next_cell_index) && loop_count == 0{
                    // if there is an agent in next cell and it has not tried
                    // to move yet (because it is first loop) then wait for
                    // it to try
                    self.agent_indexes_queue.push_back(agent_index);
                }
            }
        }
    }

    pub fn apply_attacks(&mut self, state: &mut State, attacks: &[Option<usize>]) {
    }

    fn move_agent(&self, state: &mut State, agent_index: usize, x: u8, y: u8) {
        let agent = state.agents[agent_index].as_mut().unwrap();
        state.agent_bitboard.turn_off(Parameters::coord_to_index(agent.get_x(), agent.get_y(), self.parameters.get_width()));
    }

    pub fn new() -> Self {
        let mut agent_indexes_queue = std::collections::VecDeque::new();

        for agent_index in 0..Parameters::AGENT_COUNT_MAX {
            agent_indexes_queue.push_back(agent_index);
        }

        Self {
            agent_indexes_queue: agent_indexes_queue,
            moves_applied: std::collections::HashMap::with_capacity(Parameters::AGENT_COUNT_MAX),
        }
    }
}
