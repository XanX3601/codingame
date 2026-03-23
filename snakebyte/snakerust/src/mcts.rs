use crate::action;
use crate::bitboard;
use crate::snake;
use crate::state;
use crate::zobrist;

pub struct Mcts {
    exploration: f32,
    simulator: state::GameSimulator,
    transposition_table: TranspositionTable,
}

impl Mcts {
    pub fn new() -> Self {
        Self {
            exploration: 1.414,
            simulator: state::GameSimulator::new(),
            transposition_table: TranspositionTable::new(),
        }
    }

    pub fn get_best_action(&self, game_state: &state::GameState) -> [Option<action::Direction>; state::GameDefinition::MAX_SNAKE_COUNT] {
        let node = self.transposition_table.get_node(game_state.get_zobrist_hash());
        let mut actions = [None; state::GameDefinition::MAX_SNAKE_COUNT];

        for snake_id in 0..state::GameDefinition::MAX_SNAKE_COUNT {
            if game_state.get_snake(snake_id).is_none() {continue;}

            let mut best_direction = action::Direction::Up;
            let mut max_visit_count = 0;

            for direction in action::Direction::DIRECTIONS {
                let visit_count = node.stats[snake_id][direction as usize].1;
                if visit_count > max_visit_count {
                    max_visit_count = visit_count;
                    best_direction = direction;
                }
            }

            actions[snake_id] = Some(best_direction);
        }

        actions
    }

    pub fn run(
        &mut self,
        root_state: &state::GameState,
        game_definition: &state::GameDefinition,
        zobrist_table: &zobrist::ZobristTable,
        bitboard_masks: &bitboard::BitboardMasks,
        time_limit_ms: u128
    ) -> [Option<action::Direction>; state::GameDefinition::MAX_SNAKE_COUNT] {
        let start = std::time::Instant::now();
        let mut path_hashes = [0u64; 256];
        let mut path_actions = [[None; state::GameDefinition::MAX_SNAKE_COUNT]; 256];

        let mut iteration = 0; // REMOVE
        while start.elapsed().as_millis() < time_limit_ms {
            iteration += 1;

            let mut state = root_state.clone();
            let mut turn_count = 0;

            // selection
            while turn_count < 200 {
                // find the node linked to the current state
                let node = self.transposition_table.get_node(
                    state.get_zobrist_hash()
                );

                // if the node has never been visited, go to expand
                if node.game_state_zobrist_hash != state.get_zobrist_hash()
                    || node.visit_count == 0 
                {
                    break;
                }

                // otherwise, find the best action for each snake and go on to
                // next state
                let actions = self.select_actions_decoupled(node, &state, &game_definition);
                path_hashes[turn_count] = state.get_zobrist_hash();
                path_actions[turn_count] = actions;

                state = self.simulator.apply(
                    &state,
                    actions,
                    game_definition,
                    zobrist_table
                );

                turn_count += 1;
            }

            // expansion - this method assign the hash to the node and returns
            // it. Here we need the node to be assigned the hash
            let node = self.transposition_table.get_node_mut(state.get_zobrist_hash());
            node.visit_count += 1;

            // evaluate
            let scores = state.evaluate(game_definition, bitboard_masks);

            // backpropagation
            for turn_index in 0..turn_count {
                let game_state_zobrist_hash = path_hashes[turn_index];
                let actions = path_actions[turn_index];
                let node = self.transposition_table.get_node_mut(game_state_zobrist_hash);

                node.visit_count += 1;
                for snake_id in 0..state::GameDefinition::MAX_SNAKE_COUNT {
                    if root_state.get_snake(snake_id).is_none() {continue;}
                    if let Some(direction) = actions[snake_id] {
                        node.stats[snake_id][direction as usize].0 += scores[snake_id];
                        node.stats[snake_id][direction as usize].1 += 1;
                    }
                }
            }
        }

        eprintln!("made {iteration} iterations");
        self.get_best_action(root_state)
    }

    fn select_actions_decoupled(&self, node: &Node, state: &state::GameState, game_definition: &state::GameDefinition) -> [Option<action::Direction>; state::GameDefinition::MAX_SNAKE_COUNT] {
        let mut actions = [None; state::GameDefinition::MAX_SNAKE_COUNT];
        let ln_visit_count = (node.visit_count as f32).ln();

        for snake_id in 0..state::GameDefinition::MAX_SNAKE_COUNT {
            if let Some(snake) = state.get_snake(snake_id) {
                let mut best_direction = snake.get_direction();
                let mut max_ucb = f32::MIN;

                for direction in action::Direction::DIRECTIONS {
                    // is the snake tries to go backward, ignore the direction
                    if direction == snake.get_direction().get_backward() {continue;}

                    // if the direction leads to a platform crash, ignore the 
                    // direction
                    let (head_x, head_y) = snake.get_head();
                    let (next_head_x, next_head_y) = direction.apply(head_x, head_y);
                    let is_next_head_in_grid = snake::Snake::is_in_grid(
                        next_head_x, 
                        next_head_y, 
                        game_definition.get_grid_width(),
                        game_definition.get_grid_height()
                    );
                    let is_next_head_a_platform = is_next_head_in_grid && game_definition
                        .get_platform_bitboard()
                        .is_on(
                            bitboard::Bitboard::coord_to_index(
                                next_head_x as u16,
                                next_head_y as u16,
                                game_definition.get_grid_width()
                            )
                        );

                    if is_next_head_a_platform {continue;}

                    let (direction_sum_score, direction_visit_count) = node.stats[snake_id][direction as usize];
                    let ucb = if direction_visit_count == 0 {
                        10000.0 // move not tried
                    } else {
                        (direction_sum_score / direction_visit_count as f32) + self.exploration * (ln_visit_count / direction_visit_count as f32).sqrt()
                    };

                    if ucb > max_ucb {
                        max_ucb = ucb;
                        best_direction = direction;
                    }
                }

                actions[snake_id] = Some(best_direction);
            }
        }

        actions
    }
}

#[derive(Copy, Clone, Debug)]
struct Node {
    pub game_state_zobrist_hash: u64,
    pub stats: [[(f32, u32); 4]; state::GameDefinition::MAX_SNAKE_COUNT],
    pub visit_count: u32,
}

struct TranspositionTable {
    nodes: std::boxed::Box<[Node]>,
}

impl TranspositionTable {
    const MASK: usize = TranspositionTable::SIZE - 1;
    const SIZE: usize = 1 << 20;

    pub fn new() -> Self {
        let vec = vec![unsafe {std::mem::zeroed::<Node>()}; TranspositionTable::SIZE];
        Self {
            nodes: vec.into_boxed_slice(),
        }
    }

    pub fn get_node(&self, game_state_zobrist_hash: u64) -> &Node {
        let node_index = (game_state_zobrist_hash as usize) & TranspositionTable::MASK;
        &self.nodes[node_index]
    }

    pub fn get_node_mut(&mut self, game_state_zobrist_hash: u64) -> &mut Node {
        let node_index = (game_state_zobrist_hash as usize) & TranspositionTable::MASK;
        let node = &mut self.nodes[node_index];

        // in case of collision, the retrieved node has a different hash than
        // the required one so erase it
        if node.game_state_zobrist_hash != game_state_zobrist_hash {
            *node = unsafe {std::mem::zeroed()};
            node.game_state_zobrist_hash = game_state_zobrist_hash;
        }

        node
    }
}

#[cfg(test)]
mod test {
    use pretty_assertions::assert_eq;

    use super::*;

    #[test]
    fn can_create_transposition_table() {
        TranspositionTable::new();
    }

    #[test]
    fn can_use_node() {
        let mut node = Node{
            game_state_zobrist_hash: 0,
            stats: [[(0f32, 0); 4]; state::GameDefinition::MAX_SNAKE_COUNT],
            visit_count: 0,
        };

        let mut i = 1;
        for direction in action::Direction::DIRECTIONS {
            node.stats[0][direction as usize].1 = i;
            i += 1;
        }

        assert_eq!(node.stats[0][0].1, 1);
        assert_eq!(node.stats[0][1].1, 2);
        assert_eq!(node.stats[0][2].1, 3);
        assert_eq!(node.stats[0][3].1, 4);
    }

    #[test]
    fn can_run_mcts() {
        let zobrist_table = zobrist::ZobristTable::new();

        let input = r#"0
            18
            10
            ..................
            ..................
            ..................
            ..#............#..
            ...#....##....#...
            ..................
            ..................
            .......#..#.......
            ..#...##..##...#..
            ##################
            2
            0
            1
            2
            3
        "#;

        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);

        let game_definition = state::GameDefinition::from_buffer(reader);

        let bitboard_masks = bitboard::BitboardMasks::new(
            game_definition.get_grid_width(),
            game_definition.get_grid_height(),
        );

        let mut game_state = state::GameState::new();

        let input = r#"18
            0 2
            17 2
            4 2
            13 2
            7 4
            10 4
            6 5
            11 5
            0 1
            17 1
            5 4
            12 4
            0 5
            17 5
            5 6
            12 6
            0 7
            17 7
            4
            0 2,5:2,6:2,7
            1 15,0:15,1:15,2
            2 15,5:15,6:15,7
            3 2,0:2,1:2,2
        "#;

        let cursor = std::io::Cursor::new(input.as_bytes());
        let reader = std::io::BufReader::new(cursor);

        game_state.update_from_buffer(
            reader,
            game_definition.get_grid_width(),
            game_definition.get_grid_height(),
            &zobrist_table
        );

        let mut mcts = Mcts::new();

        let actions = mcts.run(
            &game_state,
            &game_definition,
            &zobrist_table,
            &bitboard_masks,
            45
        );
    }

    #[test]
    fn can_use_transposition_table() {
        let mut transpositiion_table = TranspositionTable::new();

        let node = transpositiion_table.get_node_mut(0);
        node.visit_count += 1;

        let node = transpositiion_table.get_node_mut(0);

        assert_eq!(node.visit_count, 1);
    }
}
