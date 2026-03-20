#[derive(Clone, Copy, Debug, PartialEq)]
pub enum Direction {
    Down = 0,
    Left = 1,
    Right = 2,
    Up = 3,
}

impl Direction {
    pub fn apply(&self, x:i32, y:i32) -> (i32, i32) {
        match self {
            Direction::Down => (x, y + 1),
            Direction::Left => (x - 1, y),
            Direction::Right => (x + 1, y),
            Direction::Up => (x, y - 1),
        }
    }

    pub fn get_backward(&self) -> Direction {
        match self {
            Direction::Down => Direction::Up,
            Direction::Left => Direction::Right,
            Direction::Right => Direction::Left,
            Direction::Up => Direction::Down,
        }
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn can_apply() {
        assert_eq!(Direction::Down.apply(0, 0), (0, 1));
        assert_eq!(Direction::Left.apply(0, 0), (-1, 0));
        assert_eq!(Direction::Right.apply(0, 0), (1, 0));
        assert_eq!(Direction::Up.apply(0, 0), (0, -1));
    }

    #[test]
    fn can_get_backward() {
        assert_eq!(Direction::Down.get_backward(), Direction::Up);
        assert_eq!(Direction::Left.get_backward(), Direction::Right);
        assert_eq!(Direction::Right.get_backward(), Direction::Left);
        assert_eq!(Direction::Up.get_backward(), Direction::Down);
    }
}
