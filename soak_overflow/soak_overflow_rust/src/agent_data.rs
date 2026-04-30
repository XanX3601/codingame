#[derive(Clone, Copy, Debug, PartialEq)]
pub struct AgentData {
    optimal_range: u8,
    player_id: u8,
    shoot_cooldown: u8,
    soaking_power: u8,
    splash_bomb_count: u8,
}

impl AgentData {
    pub fn from_buffer<R: std::io::BufRead>(mut reader: R) -> Self {
        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let mut agent_data_iter = input_line
            .trim()
            .split_whitespace()
            .into_iter();

        agent_data_iter.next();

        Self {
            player_id: agent_data_iter.next().unwrap().parse::<u8>().unwrap(), 
            shoot_cooldown: agent_data_iter.next().unwrap().parse::<u8>().unwrap(), 
            optimal_range: agent_data_iter.next().unwrap().parse::<u8>().unwrap(), 
            soaking_power: agent_data_iter.next().unwrap().parse::<u8>().unwrap(), 
            splash_bomb_count: agent_data_iter.next().unwrap().parse::<u8>().unwrap(), 
        }
    }

    pub fn get_optimal_range(&self) -> u8 {
        self.optimal_range
    }

    pub fn get_player_id(&self) -> u8 {
        self.player_id
    }

    pub fn get_shoot_cooldown(&self) -> u8 {
        self.shoot_cooldown
    }

    pub fn get_soaking_power(&self) -> u8 {
        self.soaking_power
    }

    pub fn get_splash_bomb_count(&self) -> u8 {
        self.splash_bomb_count
    }

    pub fn new(
        optimal_range: u8,
        player_id: u8,
        shoot_cooldown: u8,
        soaking_power: u8,
        splash_bomb_count: u8,
    ) -> Self {
        Self {
            optimal_range: optimal_range,
            player_id: player_id,
            shoot_cooldown: shoot_cooldown,
            soaking_power: soaking_power,
            splash_bomb_count: splash_bomb_count,
        }
    }
}
