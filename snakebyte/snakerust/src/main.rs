fn main() {
    let zobrist_table = snakerust::zobrist::ZobristTable::new();

    let game_definition = snakerust::state::GameDefinition::from_buffer(std::io::stdin().lock());
    let mut game_state = snakerust::state::GameState::new();

    loop {
        game_state.update_from_buffer(
            std::io::stdin().lock(),
            game_definition.get_grid_width(),
            game_definition.get_grid_height(),
            &zobrist_table
        );

        println!("WAIT")
    }
}
