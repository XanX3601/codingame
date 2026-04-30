use soak_overflow_rust::direction::Direction;

#[test]
fn can_apply_direction_to_coord() {
    assert_eq!(Direction::Down.apply_to(1, 1, 3, 3), (1, 2));
    assert_eq!(Direction::Left.apply_to(1, 1, 3, 3), (0, 1));
    assert_eq!(Direction::Right.apply_to(1, 1, 3, 3), (2, 1));
    assert_eq!(Direction::Up.apply_to(1, 1, 3, 3), (1, 0));
}

#[test]
fn can_not_move_out_of_grid() {
    assert_eq!(Direction::Down.apply_to(1, 3, 3, 4), (1, 3));
    assert_eq!(Direction::Left.apply_to(0, 1, 3, 4), (0, 1));
    assert_eq!(Direction::Right.apply_to(2, 1, 3, 4), (2, 1));
    assert_eq!(Direction::Up.apply_to(1, 0, 3, 4), (1, 0));
}
