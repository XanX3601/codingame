#[derive(Clone, Copy, Debug, PartialEq)]
pub struct Agent {
    shoot_cooldonw: u8,
    splash_bomb_count: u8,
    wetness: u8,
    x: u8,
    y: u8,
}

impl Agent {
    pub fn get_shoot_cooldown(&self) -> u8 {
        self.shoot_cooldonw
    }

    pub fn get_splash_bomb_count(&self) -> u8 {
        self.splash_bomb_count
    }

    pub fn get_wetness(&self) -> u8 {
        self.wetness
    }

    pub fn get_x(&self) -> u8 {
        self.x
    }

    pub fn get_y(&self) -> u8 {
        self.y
    }

    pub fn set_x(&mut self, x: u8) {
        self.x = x;
    }

    pub fn set_y(&mut self, y: u8) {
        self.y = y;
    }

    pub fn new(
        x: u8,
        y: u8,
        shoot_cooldown: u8,
        splash_bomb_count: u8,
        wetness: u8,
    ) ->Self {
        Self {
            shoot_cooldonw: shoot_cooldown,
            splash_bomb_count: splash_bomb_count,
            wetness: wetness,
            x: x,
            y: y,
        }
    }
}
