use soak_overflow_rust::agent_data::{AgentData};
use soak_overflow_rust::bitboard::Bitboard;
use soak_overflow_rust::parameters::Parameters;

#[test]
fn can_create_parameters_from_buffer() {
    let input = r#"0
        8
        1 0 1 4 16 1
        2 0 5 6 24 0
        3 0 2 2 8 3
        4 0 2 4 16 2
        5 1 1 4 16 1
        6 1 5 6 24 0
        7 1 2 2 8 3
        8 1 2 4 16 2
        16 8
        0 0 0 1 0 0 2 0 0 3 0 0 4 0 0 5 0 0 6 0 0 7 0 0 8 0 0 9 0 0 10 0 0 11 0 0 12 0 0 13 0 0 14 0 0 15 0 0
        0 1 0 1 1 0 2 1 2 3 1 0 4 1 1 5 1 0 6 1 0 7 1 0 8 1 0 9 1 2 10 1 0 11 1 0 12 1 0 13 1 1 14 1 0 15 1 0
        0 2 0 1 2 0 2 2 0 3 2 0 4 2 2 5 2 0 6 2 0 7 2 0 8 2 0 9 2 0 10 2 0 11 2 0 12 2 0 13 2 0 14 2 0 15 2 0
        0 3 0 1 3 0 2 3 0 3 3 0 4 3 1 5 3 0 6 3 0 7 3 0 8 3 0 9 3 0 10 3 0 11 3 0 12 3 0 13 3 0 14 3 0 15 3 0
        0 4 0 1 4 0 2 4 0 3 4 0 4 4 0 5 4 0 6 4 0 7 4 0 8 4 0 9 4 0 10 4 0 11 4 1 12 4 0 13 4 0 14 4 0 15 4 0
        0 5 0 1 5 0 2 5 0 3 5 0 4 5 0 5 5 0 6 5 0 7 5 0 8 5 0 9 5 0 10 5 0 11 5 2 12 5 0 13 5 0 14 5 0 15 5 0
        0 6 0 1 6 0 2 6 1 3 6 0 4 6 0 5 6 0 6 6 2 7 6 0 8 6 0 9 6 0 10 6 0 11 6 1 12 6 0 13 6 2 14 6 0 15 6 0
        0 7 0 1 7 0 2 7 0 3 7 0 4 7 0 5 7 0 6 7 0 7 7 0 8 7 0 9 7 0 10 7 0 11 7 0 12 7 0 13 7 0 14 7 0 15 7 0
    "#;

    let cursor = std::io::Cursor::new(input.as_bytes());
    let reader = std::io::BufReader::new(cursor);

    let parameters = Parameters::from_buffer(reader);

    assert_eq!(parameters.get_my_id(), 0);
    assert_eq!(parameters.get_agent_count(), 8);
    assert_eq!(parameters.get_width(), 16);
    assert_eq!(parameters.get_height(), 8);

    let expected_agent_data = AgentData::new(4, 0, 1, 16, 1);
    assert_eq!(parameters.get_agent_data(1), &Some(expected_agent_data));
    let expected_agent_data = AgentData::new(6, 0, 5, 24, 0);
    assert_eq!(parameters.get_agent_data(2), &Some(expected_agent_data));
    let expected_agent_data = AgentData::new(2, 0, 2, 8, 3);
    assert_eq!(parameters.get_agent_data(3), &Some(expected_agent_data));
    let expected_agent_data = AgentData::new(4, 0, 2, 16, 2);
    assert_eq!(parameters.get_agent_data(4), &Some(expected_agent_data));
    let expected_agent_data = AgentData::new(4, 1, 1, 16, 1);
    assert_eq!(parameters.get_agent_data(5), &Some(expected_agent_data));
    let expected_agent_data = AgentData::new(6, 1, 5, 24, 0);
    assert_eq!(parameters.get_agent_data(6), &Some(expected_agent_data));
    let expected_agent_data = AgentData::new(2, 1, 2, 8, 3);
    assert_eq!(parameters.get_agent_data(7), &Some(expected_agent_data));
    let expected_agent_data = AgentData::new(4, 1, 2, 16, 2);
    assert_eq!(parameters.get_agent_data(8), &Some(expected_agent_data));

    let mut expected_empty_cell_bitboard = Bitboard::new();
    let mut expected_high_cover_bitboard = Bitboard::new();
    let mut expected_low_cover_bitboard = Bitboard::new();
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(0, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(1, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(2, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(3, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(4, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(5, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(6, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(7, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(8, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(9, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(10, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(11, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(12, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(13, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(14, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(15, 0, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(0, 1, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(1, 1, 16));
    expected_high_cover_bitboard.turn_on(Parameters::coord_to_index(2, 1, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(3, 1, 16));
    expected_low_cover_bitboard.turn_on(Parameters::coord_to_index(4, 1, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(5, 1, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(6, 1, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(7, 1, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(8, 1, 16));
    expected_high_cover_bitboard.turn_on(Parameters::coord_to_index(9, 1, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(10, 1, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(11, 1, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(12, 1, 16));
    expected_low_cover_bitboard.turn_on(Parameters::coord_to_index(13, 1, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(14, 1, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(15, 1, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(0, 2, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(1, 2, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(2, 2, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(3, 2, 16));
    expected_high_cover_bitboard.turn_on(Parameters::coord_to_index(4, 2, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(5, 2, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(6, 2, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(7, 2, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(8, 2, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(9, 2, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(10, 2, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(11, 2, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(12, 2, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(13, 2, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(14, 2, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(15, 2, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(0, 3, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(1, 3, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(2, 3, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(3, 3, 16));
    expected_low_cover_bitboard.turn_on(Parameters::coord_to_index(4, 3, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(5, 3, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(6, 3, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(7, 3, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(8, 3, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(9, 3, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(10, 3, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(11, 3, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(12, 3, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(13, 3, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(14, 3, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(15, 3, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(0, 4, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(1, 4, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(2, 4, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(3, 4, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(4, 4, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(5, 4, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(6, 4, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(7, 4, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(8, 4, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(9, 4, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(10, 4, 16));
    expected_low_cover_bitboard.turn_on(Parameters::coord_to_index(11, 4, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(12, 4, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(13, 4, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(14, 4, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(15, 4, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(0, 5, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(1, 5, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(2, 5, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(3, 5, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(4, 5, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(5, 5, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(6, 5, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(7, 5, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(8, 5, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(9, 5, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(10, 5, 16));
    expected_high_cover_bitboard.turn_on(Parameters::coord_to_index(11, 5, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(12, 5, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(13, 5, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(14, 5, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(15, 5, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(0, 6, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(1, 6, 16));
    expected_low_cover_bitboard.turn_on(Parameters::coord_to_index(2, 6, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(3, 6, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(4, 6, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(5, 6, 16));
    expected_high_cover_bitboard.turn_on(Parameters::coord_to_index(6, 6, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(7, 6, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(8, 6, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(9, 6, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(10, 6, 16));
    expected_low_cover_bitboard.turn_on(Parameters::coord_to_index(11, 6, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(12, 6, 16));
    expected_high_cover_bitboard.turn_on(Parameters::coord_to_index(13, 6, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(14, 6, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(15, 6, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(0, 7, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(1, 7, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(2, 7, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(3, 7, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(4, 7, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(5, 7, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(6, 7, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(7, 7, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(8, 7, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(9, 7, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(10, 7, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(11, 7, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(12, 7, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(13, 7, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(14, 7, 16));
    expected_empty_cell_bitboard.turn_on(Parameters::coord_to_index(15, 7, 16));
    assert_eq!(parameters.get_empty_cell_bitboard(), &expected_empty_cell_bitboard);
    assert_eq!(parameters.get_low_cover_bitboard(), &expected_low_cover_bitboard);
    assert_eq!(parameters.get_high_cover_bitboard(), &expected_high_cover_bitboard);

    let expected_cover_bitboard = expected_low_cover_bitboard.or(
        &expected_high_cover_bitboard
    );

    assert_eq!(parameters.get_cover_bitboard(), &expected_cover_bitboard);
}
