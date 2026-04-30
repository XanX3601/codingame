#[derive(Clone, Copy, Debug, PartialEq)]
pub enum Direction {
    Down,
    Left,
    Right,
    Up,
}

impl Direction {
    pub fn apply_to(&self, x: u8, y: u8, width: u8, height: u8) -> (u8, u8) {
        match self {
            Self::Down => (x, if y < height - 1 {y + 1} else {y}),
            Self::Left => (if x > 0 {x - 1} else {x}, y),
            Self::Right => (if x < width - 1 {x + 1} else {x}, y),
            Self::Up => (x, if y > 0 {y - 1} else {y}),
        }
    }
}
