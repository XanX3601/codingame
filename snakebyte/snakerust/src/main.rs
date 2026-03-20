fn main() {
    let game_definition = snakerust::state::GameDefinition::from_buffer(std::io::stdin().lock());
    let mut game_state = snakerust::state::GameState::new(&game_definition);


    loop {
        game_state.update_from_buffer(std::io::stdin().lock());

        println!("WAIT")
    }
}
