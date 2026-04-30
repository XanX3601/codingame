use soak_overflow_rust::agent::Agent;

#[test]
fn can_create_agent() {
    let agent = Agent::new(0, 1, 2, 3, 4);

    assert_eq!(agent.get_shoot_cooldown(), 2);
    assert_eq!(agent.get_splash_bomb_count(), 3);
    assert_eq!(agent.get_wetness(), 4);
    assert_eq!(agent.get_x(), 0);
    assert_eq!(agent.get_y(), 1);
}
