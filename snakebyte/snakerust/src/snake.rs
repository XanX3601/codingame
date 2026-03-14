#[derive(Clone, Debug, PartialEq)]
pub struct Snake {
}

#[cfg(test)]
mod test {
    #[test]
    fn can_create_snake() {
        super::Snake{};
    }
}
