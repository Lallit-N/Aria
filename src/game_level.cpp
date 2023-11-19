#include "game_level.hpp" 
#include <random>

/*
	CREATING WALLS:
		Push back a pair to terrains:
			make_pair(vec4(x_coord, y_coord, length, width), moveable?))

	CREATING FLOORS:
		Push back a vec2 to floors:
			vec2(x_coord, y_coord)

	CREATING ENEMIES:
		Push back a pair of pos and enemy attributes to enemies:
			make_pair(vec2(x_coord, y_coord), ENEMY_OBJECT);
*/

// Helper function to get a random normal damage enemy
const Enemy& getRandomNormalEnemy() {
	static const std::vector<Enemy> normalEnemies = { WATER_NORMAL, FIRE_NORMAL, EARTH_NORMAL, LIGHTNING_NORMAL };

	// Use C++11 random number generation
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(0, normalEnemies.size() - 1);

	return normalEnemies[distribution(gen)];
}

bool GameLevel::init(uint level) {
	printf("Initializing game for level: %i\n", level);
	if (level < 0) {
		return false;
	}
	this->curr_level = level;

	std::vector<std::string>& texts = this->texts;
	std::vector<std::array<float, TEXT_ATTRIBUTES>>& text_attrs = this->text_attrs;
	std::vector<std::array< vec2, OBSTACLE_ATTRIBUTES>>& obstacles = this->obstacle_attrs;
	std::vector<vec2>& floors = this->floor_pos;
	std::vector<std::pair<vec4, bool>>& terrains = this->terrains_attr;
	std::vector<std::pair<vec2, Enemy>>& enemies = this->enemies_attr;

	texts.clear();
	text_attrs.clear();
	floors.clear();
	terrains.clear();
	enemies.clear();
	obstacles.clear();

	switch (level) {
	case TUTORIAL:
		for (uint i = 0; i < 5; i++) {
			for (uint j = 0; j < 3; j++) {
				floors.push_back(vec2(100 + i * 250, 100 + j * 250));
			}
		}

		this->player_starting_pos = vec2(200, 200);
		this->exit_door_pos = vec2(1200, 700);

		texts.push_back("Use WASD to move around");
		text_attrs.push_back({0.f,175.f,1.0f,1.0f,1.0f,0.f});
		texts.push_back("Use M1 button to shoot");
		text_attrs.push_back({0.f,125.f,1.0f,1.0f,1.0f,0.f});
		texts.push_back("Use 1,2,3,4 to cycle through elements");
		text_attrs.push_back({0.f,75.f,1.0f,1.0f,1.0f,0.f});		
		texts.push_back("Move to the exit door when ready");
		text_attrs.push_back({0.f,25.f,1.0f,1.0f,1.0f,0.f});


		terrains.push_back(std::make_pair(vec4(0, 0, 1400, 100), false));
		terrains.push_back(std::make_pair(vec4(0, 100, 100, 700), false));
		terrains.push_back(std::make_pair(vec4(0, 800, 1400, 100), false));
		terrains.push_back(std::make_pair(vec4(1300, 100, 100, 700), false));
		break;

	case LEVEL_1:
		this->player_starting_pos = vec2(200, 700);
		this->exit_door_pos = vec2(1450, 900);

		// I think we should make a helper for pushing back into terrains vector, thoughts? - Leo
		terrains.push_back(std::make_pair(vec4(0, -50, 1800, 100), false));
		terrains.push_back(std::make_pair(vec4(0, 50, 100, 900), false));
		terrains.push_back(std::make_pair(vec4(0, 950, 1800, 100), false));
		terrains.push_back(std::make_pair(vec4(1700, 50, 100, 900), false));
		terrains.push_back(std::make_pair(vec4(350, 400, 100, 600), false));
		terrains.push_back(std::make_pair(vec4(750, 0, 100, 600), false));
		terrains.push_back(std::make_pair(vec4(1150, 400, 100, 600), false));

		enemies.push_back(std::make_pair(vec2(350, 250), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(700, 800), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(950, 800), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(1100, 250), getRandomNormalEnemy()));
		break;

	// Same as level 1 but with moving walls
	case LEVEL_2:
		this->player_starting_pos = vec2(200, 700);

		texts.push_back("Don't get haunted by the ghosts!");
		text_attrs.push_back({ 0.f,80.f,1.0f,1.0f,1.0f,0.f });
		
		//TODO: WHY DOES GHOST PERSIST?
		obstacles.push_back({ vec2(150, 450), vec2(80,80), vec2(100.f,0.f) });

		terrains.push_back(std::make_pair(vec4(0, -50, 1800, 100), false));
		terrains.push_back(std::make_pair(vec4(0, 50, 100, 900), false));
		terrains.push_back(std::make_pair(vec4(0, 950, 1800, 100), false));
		terrains.push_back(std::make_pair(vec4(1700, 50, 100, 900), false));
		terrains.push_back(std::make_pair(vec4(350, 400, 100, 600), false));
		terrains.push_back(std::make_pair(vec4(759, 0, 100, 600), false));
		terrains.push_back(std::make_pair(vec4(1150, 400, 100, 600), false));

		terrains.push_back(std::make_pair(vec4(525, 450, 75, 75), true));
		terrains.push_back(std::make_pair(vec4(775, 450, 75, 75), true));
		terrains.push_back(std::make_pair(vec4(1125, 450, 75, 75), true));


		enemies.push_back(std::make_pair(vec2(350, 250), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(700, 800), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(950, 800), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(1100, 250), getRandomNormalEnemy()));

		for (uint i = 0; i < 6; i++) {
			for (uint j = 0; j < 4; j++) {
				floors.push_back(vec2(100 + i * 250, 50 + j * 250));
			}
		}
		for (uint i = 0; i < 4; i++) {
			floors.push_back(vec2(1475, 50 + i * 250));
		}

		this->exit_door_pos = vec2(1450, 900);
		break;

	case LEVEL_3:
		this->player_starting_pos = vec2(200, 200);
		this->exit_door_pos = vec2(3900, 1900);

		for (uint i = 0; i < 16; i++) {
			for (uint j = 0; j < 8; j++) {
				floors.push_back(vec2(i * 250, j * 250));
			}
		}

		//Surrounding box walls
		terrains.push_back(std::make_pair(vec4(0, -100, 4000, 100), false));
		terrains.push_back(std::make_pair(vec4(-100, -100, 100, 2200), false));
		terrains.push_back(std::make_pair(vec4(4000, -100, 100, 2200), false));
		terrains.push_back(std::make_pair(vec4(0, 2000, 4000, 100), false));

		//inner walls
		terrains.push_back(std::make_pair(vec4(0, 700, 1000, 1300), false));
		terrains.push_back(std::make_pair(vec4(1000, 900, 700, 1100), false));
		terrains.push_back(std::make_pair(vec4(1700, 0, 500, 400), false));
		terrains.push_back(std::make_pair(vec4(1700, 700, 1600, 400), false));
		terrains.push_back(std::make_pair(vec4(2200, 1400, 1800, 100), false));

		terrains.push_back(std::make_pair(vec4(3800, 700, 75, 75), true));
		terrains.push_back(std::make_pair(vec4(3600, 800, 75, 75), true));
		terrains.push_back(std::make_pair(vec4(3400, 900, 75, 75), true));

		// Area 1
		enemies.push_back(std::make_pair(vec2(1200, 700), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(1500, 500), getRandomNormalEnemy()));

		// Area 2
		enemies.push_back(std::make_pair(vec2(2400, 200), WATER_NORMAL));
		enemies.push_back(std::make_pair(vec2(3200, 200), FIRE_NORMAL));
		enemies.push_back(std::make_pair(vec2(2800, 500), EARTH_NORMAL));
		enemies.push_back(std::make_pair(vec2(3600, 500), LIGHTNING_NORMAL));

		// Area 3
		enemies.push_back(std::make_pair(vec2(2400, 1250), EARTH_NORMAL));
		enemies.push_back(std::make_pair(vec2(3000, 1250), EARTH_NORMAL));

		// Area 4
		enemies.push_back(std::make_pair(vec2(3200, 1600), WATER_NORMAL));
		enemies.push_back(std::make_pair(vec2(3200, 1700), WATER_NORMAL));
		enemies.push_back(std::make_pair(vec2(3200, 1800), WATER_NORMAL));
		enemies.push_back(std::make_pair(vec2(3200, 1900), WATER_NORMAL));


		break;
	case POWER_UP:
		for (uint i = 0; i < 5; i++) {
			for (uint j = 0; j < 3; j++) {
				floors.push_back(vec2(100 + i * 250, 100 + j * 250));
			}
		}

		this->player_starting_pos = vec2(500, 500);
		this->exit_door_pos = vec2(1200, 700);

		texts.push_back("Shoot the mystery boxes to power up!");
		text_attrs.push_back({ 0.f,150.f,1.0f,1.0f,1.0f,0.f });

		texts.push_back("Choose wisely... you can only keep one!");
		text_attrs.push_back({ 0.f,100.f,1.0f,1.0f,1.0f,0.f });


		terrains.push_back(std::make_pair(vec4(0, 0, 1400, 100), false));
		terrains.push_back(std::make_pair(vec4(0, 100, 100, 700), false));
		terrains.push_back(std::make_pair(vec4(0, 800, 1400, 100), false));
		terrains.push_back(std::make_pair(vec4(1300, 100, 100, 700), false));
		break;
	
	default:
		printf("no level provided\n");
		break;
	}

	return true;
}