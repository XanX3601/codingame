fn main() {
    let stdin = std::io::stdin();
    let handle = stdin.lock();
    let reader = std::io::BufReader::new(handle);

    let game_definition = snakerust::state::GameDefinition::from_buffer(reader);

    eprintln!("{game_definition:?}");

    while true {
        println!("WAIT")
    }
}
