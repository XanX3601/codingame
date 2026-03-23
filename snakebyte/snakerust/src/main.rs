fn main() {
    let zobrist_table = snakerust::zobrist::ZobristTable::new();

    let game_definition = snakerust::state::GameDefinition::from_buffer(std::io::stdin().lock());
    let bitboard_masks = snakerust::bitboard::BitboardMasks::new(
        game_definition.get_grid_width(),
        game_definition.get_grid_height(),
    );
    let mut mcts = snakerust::mcts::Mcts::new();

    let mut game_state = snakerust::state::GameState::new();

    loop {
        game_state.update_from_buffer(
            std::io::stdin().lock(),
            game_definition.get_grid_width(),
            game_definition.get_grid_height(),
            &zobrist_table
        );

        let actions = mcts.run(
            &game_state,
            &game_definition,
            &zobrist_table,
            &bitboard_masks,
            45
        );

        for my_snake_id in game_definition.get_my_snake_ids().iter() {
            if let Some(direction) = actions[*my_snake_id as usize] {
                print!("{my_snake_id} {}", direction.to_string());
            }
        }
    }
}
