use soak_overflow_rust::agent_data::AgentData;

#[test]
fn can_create_agent_data() {
    let agent_data = AgentData::new(1, 2, 3, 4, 5);

    assert_eq!(agent_data.get_optimal_range(), 1);
    assert_eq!(agent_data.get_player_id(), 2);
    assert_eq!(agent_data.get_shoot_cooldown(), 3);
    assert_eq!(agent_data.get_soaking_power(), 4);
    assert_eq!(agent_data.get_splash_bomb_count(), 5);
}

#[test]
fn can_create_agent_data_from_buffer() {
    let input = r#"0 1 2 3 4 5
    "#;

    let cursor = std::io::Cursor::new(input.as_bytes());
    let reader = std::io::BufReader::new(cursor);

    let agent_data = AgentData::from_buffer(reader);

    assert_eq!(agent_data.get_player_id(), 1);
    assert_eq!(agent_data.get_shoot_cooldown(), 2);
    assert_eq!(agent_data.get_optimal_range(), 3);
    assert_eq!(agent_data.get_soaking_power(), 4);
    assert_eq!(agent_data.get_splash_bomb_count(), 5);
}
