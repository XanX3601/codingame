use soak_overflow_rust::{agent::Agent, bitboard::Bitboard, parameters::Parameters, state::State};

#[test]
fn can_create_state_from_buffer() {
    let input = r#"8
        1 0 0 0 1 0
        2 0 5 0 0 0
        3 0 1 0 3 0
        4 0 6 0 2 0
        5 0 3 0 1 0
        6 13 6 0 1 0
        7 13 1 0 0 0
        8 13 5 0 3 0
        5
    "#;

    let cursor = std::io::Cursor::new(input.as_bytes());
    let reader = std::io::BufReader::new(cursor);

    let state = State::from_buffer(reader, 10);

    let mut expected_agent_bitboard = Bitboard::new();
    expected_agent_bitboard.turn_on(Parameters::coord_to_index(0, 0, 10));
    expected_agent_bitboard.turn_on(Parameters::coord_to_index(0, 5, 10));
    expected_agent_bitboard.turn_on(Parameters::coord_to_index(0, 1, 10));
    expected_agent_bitboard.turn_on(Parameters::coord_to_index(0, 6, 10));
    expected_agent_bitboard.turn_on(Parameters::coord_to_index(0, 3, 10));
    expected_agent_bitboard.turn_on(Parameters::coord_to_index(13, 6, 10));
    expected_agent_bitboard.turn_on(Parameters::coord_to_index(13, 1, 10));
    expected_agent_bitboard.turn_on(Parameters::coord_to_index(13, 5, 10));

    assert_eq!(state.get_agent_bitboard(), &expected_agent_bitboard);

    let expected_agent = Agent::new(0, 0, 0, 1, 0);
    assert_eq!(*state.get_agent(1), Some(expected_agent));
    let expected_agent = Agent::new(0, 5, 0, 0, 0);
    assert_eq!(*state.get_agent(2), Some(expected_agent));
    let expected_agent = Agent::new(0, 1, 0, 3, 0);
    assert_eq!(*state.get_agent(3), Some(expected_agent));
    let expected_agent = Agent::new(0, 6, 0, 2, 0);
    assert_eq!(*state.get_agent(4), Some(expected_agent));
    let expected_agent = Agent::new(0, 3, 0, 1, 0);
    assert_eq!(*state.get_agent(5), Some(expected_agent));
    let expected_agent = Agent::new(13, 6, 0, 1, 0);
    assert_eq!(*state.get_agent(6), Some(expected_agent));
    let expected_agent = Agent::new(13, 1, 0, 0, 0);
    assert_eq!(*state.get_agent(7), Some(expected_agent));
    let expected_agent = Agent::new(13, 5, 0, 3, 0);
    assert_eq!(*state.get_agent(8), Some(expected_agent));
    assert_eq!(*state.get_agent(9), None);
    assert_eq!(*state.get_agent(10), None);
}
