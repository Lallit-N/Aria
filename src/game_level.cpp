#include "game_level.hpp" 
#include <random>

/*
	CREATING WALLS:
		Push back a pair to terrains:
			make_pair(vec4(x_coord, y_coord, length, width), Terrain)))
			Honestly, implementation is a bit messy right now (could think of refactoring)

	CREATING FLOORS:
		Push back a vec2 to floors:
			vec2(x_coord, y_coord)

	CREATING ENEMIES:
		Push back a pair of pos and enemy attributes to enemies:
			make_pair(vec2(x_coord, y_coord), ENEMY_OBJECT);

	CREATING HEALTH PACKS:
		Push back a vec2 to health_packs_pos
			vec2(x_coord, y_coord)
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

	std::vector<vec2>& health_packs_pos = this->health_packs_pos;
	std::vector<std::string>& texts = this->texts;
	std::vector<std::array<float, TEXT_ATTRIBUTES>>& text_attrs = this->text_attrs;
	std::vector<std::array< vec2, OBSTACLE_ATTRIBUTES>>& obstacles = this->obstacle_attrs;
	std::vector<vec4>& floors = this->floor_attrs;
	std::vector<std::pair<vec4, Terrain>>& terrains = this->terrains_attr;
	std::vector<std::pair<vec2, Enemy>>& enemies = this->enemies_attr;
	std::vector<std::pair<vec2, Enemy>>& bosses = this->bosses_attr;

	health_packs_pos.clear();
	texts.clear();
	text_attrs.clear();
	floors.clear();
	terrains.clear();
	enemies.clear();
	obstacles.clear();
	bosses.clear();

	switch (level) {
	case TUTORIAL:
		floors.push_back(vec4(25, 30, 1300, 650));

		this->player_starting_pos = vec2(200, 200);
		this->exit_door_pos = vec2(1225, 575);

		health_packs_pos.push_back(vec2(400, 300));
		health_packs_pos.push_back(vec2(600, 400));
		health_packs_pos.push_back(vec2(800, 500));
		health_packs_pos.push_back(vec2(1000, 600));

		texts.push_back("Use WASD to move around");
		text_attrs.push_back({0.f,225.f,1.0f,1.0f,1.0f,0.f});
		texts.push_back("Use M1 button to shoot");
		text_attrs.push_back({0.f,175.f,1.0f,1.0f,1.0f,0.f});
		texts.push_back("Use 1,2,3,4 to cycle through elements");
		text_attrs.push_back({0.f,125.f,1.0f,1.0f,1.0f,0.f});		
		texts.push_back("Collect health packs to heal!");
		text_attrs.push_back({0.f,75.f,1.0f,1.0f,1.0f,0.f});		
		texts.push_back("Move to the exit door when ready");
		text_attrs.push_back({0.f,25.f,1.0f,1.0f,1.0f,0.f});


		terrains.push_back(std::make_pair(vec4(25, 0, 1300, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 675, 1300, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 700), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1325, 0, default_side_width, 700), SIDE_STATIONARY));
		break;

	case LEVEL_1:
		this->player_starting_pos = vec2(200, 700);
		this->exit_door_pos = vec2(1450, 775);

		floors.push_back(vec4(25, 25, 1800, 875));

		// I think we should make a helper for pushing back into terrains vector, thoughts? - Leo
		terrains.push_back(std::make_pair(vec4(25, 0, 1800, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 875, 1800, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 900), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1825, 0, default_side_width, 900), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(350, 400, default_side_width, 475), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(750, 0, default_side_width, 475), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1150, 400, default_side_width, 475), SIDE_STATIONARY));

		enemies.push_back(std::make_pair(vec2(350, 250), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(700, 800), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(950, 800), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(1100, 250), FIRE_NORMAL)); // last enemy must be fire
		break;
	
	case FIRE_BOSS:
		floors.push_back(vec4(25, 25, 2700, 1375));

		this->player_starting_pos = vec2(800, 650);
		this->exit_door_pos = NULL_POS;

		terrains.push_back(std::make_pair(vec4(25, 0, 2700, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 1375, 2700, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 1400), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2725, 0, default_side_width, 1400), SIDE_STATIONARY));

		bosses.push_back(std::make_pair(vec2(1400, 700), FIRE_HIGH_DAMAGE));
		break;
	
	case LEVEL_2: // Same as level 1 but with moving walls
		this->player_starting_pos = vec2(200, 700);

		texts.push_back("Don't get haunted by the ghosts!");
		text_attrs.push_back({ 0.f,80.f,1.0f,1.0f,1.0f,0.f });
		
		obstacles.push_back({ vec2(150, 450), vec2(80,80), vec2(100.f,0.f) });

		floors.push_back(vec4(25, 25, 1800, 875));

		terrains.push_back(std::make_pair(vec4(25, 0, 1800, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 875, 1800, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 900), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1825, 0, default_side_width, 900), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(350, 400, default_side_width, 475), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(750, 0, default_side_width, 475), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1150, 400, default_side_width, 475), SIDE_STATIONARY));

		terrains.push_back(std::make_pair(vec4(525, 450, 50, 50), GENERIC_MOVABLE));
		terrains.push_back(std::make_pair(vec4(775, 450, 50, 50), GENERIC_MOVABLE));
		terrains.push_back(std::make_pair(vec4(1125, 450, 50, 50), GENERIC_MOVABLE));


		enemies.push_back(std::make_pair(vec2(350, 250), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(700, 800), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(950, 800), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(1100, 250), EARTH_NORMAL)); // last enemy must be earth

		this->exit_door_pos = vec2(1450, 775);
		break;

	case EARTH_BOSS:
		floors.push_back(vec4(25, 25, 2700, 1375));

		this->player_starting_pos = vec2(800, 650);
		this->exit_door_pos = NULL_POS;

		terrains.push_back(std::make_pair(vec4(25, 0, 2700, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 1375, 2700, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 1400), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2725, 0, default_side_width, 1400), SIDE_STATIONARY));

		bosses.push_back(std::make_pair(vec2(1400, 700), EARTH_HIGH_DAMAGE));
		break;

	case LEVEL_3: // Same as level 1 but with moving walls
		this->player_starting_pos = vec2(200, 700);

		texts.push_back("Don't get haunted by the ghosts!");
		text_attrs.push_back({ 0.f,80.f,1.0f,1.0f,1.0f,0.f });
		
		floors.push_back(vec4(25, 25, 1800, 875));

		obstacles.push_back({ vec2(150, 450), vec2(80,80), vec2(100.f,0.f) });

		terrains.push_back(std::make_pair(vec4(25, 0, 1800, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 875, 1800, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 900), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1825, 0, default_side_width, 900), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(350, 400, default_side_width, 475), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(750, 0, default_side_width, 475), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1150, 400, default_side_width, 475), SIDE_STATIONARY));

		terrains.push_back(std::make_pair(vec4(525, 450, 50, 50), GENERIC_MOVABLE));
		terrains.push_back(std::make_pair(vec4(775, 450, 50, 50), GENERIC_MOVABLE));
		terrains.push_back(std::make_pair(vec4(1125, 450, 50, 50), GENERIC_MOVABLE));

		enemies.push_back(std::make_pair(vec2(350, 250), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(700, 800), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(950, 800), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(1100, 250), LIGHTNING_NORMAL)); // last enemy must be lightning


		this->exit_door_pos = vec2(1450, 775);
		break;

	case LIGHTNING_BOSS:
		floors.push_back(vec4(25, 25, 2700, 1375));

		this->player_starting_pos = vec2(800, 650);
		this->exit_door_pos = NULL_POS;

		terrains.push_back(std::make_pair(vec4(25, 0, 2700, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 1375, 2700, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 1400), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2725, 0, default_side_width, 1400), SIDE_STATIONARY));

		bosses.push_back(std::make_pair(vec2(1400, 700), LIGHTNING_HIGH_DAMAGE));
		break;

	case LEVEL_4:
		this->player_starting_pos = vec2(200, 200);
		this->exit_door_pos = vec2(3900, 1875);

		floors.push_back(vec4(0, 0, 4000, 2000));

		//Surrounding box walls
		terrains.push_back(std::make_pair(vec4(0, -100, 4000, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 1975, 4000, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(-25, -100, default_side_width, 2100), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(4000, -100, default_side_width, 2100), SIDE_STATIONARY));

		//inner walls
		terrains.push_back(std::make_pair(vec4(0, 700, 1000, 1300), GENERIC_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1000, 900, 700, 1100), GENERIC_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1700, 0, 500, 400), GENERIC_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1700, 700, 1600, 400), GENERIC_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2200, 1400, 1800, 100), NORTH_STATIONARY));

		terrains.push_back(std::make_pair(vec4(3800, 700, 75, 75), GENERIC_MOVABLE));
		terrains.push_back(std::make_pair(vec4(3600, 800, 75, 75), GENERIC_MOVABLE));
		terrains.push_back(std::make_pair(vec4(3400, 900, 75, 75), GENERIC_MOVABLE));

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
		enemies.push_back(std::make_pair(vec2(3200, 1600), WATER_NORMAL));	// final group of water enemies before "final boss"
		enemies.push_back(std::make_pair(vec2(3200, 1700), WATER_NORMAL));
		enemies.push_back(std::make_pair(vec2(3200, 1800), WATER_NORMAL));
		enemies.push_back(std::make_pair(vec2(3200, 1900), WATER_NORMAL));

		break;

	case LEVEL_LEO:
		this->player_starting_pos = vec2(1000, 400);
		this->exit_door_pos = vec2(3900, 1875);

		// Spawn Area
		terrains.push_back(std::make_pair(vec4(500, 0, 1000, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(475, 0, default_side_width, 200), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(475, 500, default_side_width, 300), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(500, 775, 1000, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1500, 0, default_side_width, 200), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1500, 500, default_side_width, 300), SIDE_STATIONARY));
		floors.push_back(vec4(500, 0, 1000, 800));

		// Left Elbow Hallway from Spawn
		terrains.push_back(std::make_pair(vec4(-200, 200, 700, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 500, 500, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(-225, 200, default_side_width, 700), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 500, default_side_width, 500), SIDE_STATIONARY));
		floors.push_back(vec4(-200, 200, 700, 300));
		floors.push_back(vec4(-200, 500, 200, 500));

		// Room Under Left Elbow Hallway
		terrains.push_back(std::make_pair(vec4(0, 900, 500, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(500, 900, default_side_width, 300), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(500, 1400, default_side_width, 300), SIDE_STATIONARY));

		terrains.push_back(std::make_pair(vec4(-700, 900, 500, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(-725, 900, default_side_width, 800), SIDE_STATIONARY));

		terrains.push_back(std::make_pair(vec4(-700, 1675, 1200, default_south_height), SOUTH_STATIONARY));
		
		floors.push_back(vec4(-700, 900, 1200, 800));

		enemies.push_back(std::make_pair(vec2(-500, 1600), WATER_NORMAL));
		enemies.push_back(std::make_pair(vec2(-300, 1600), WATER_NORMAL));
		enemies.push_back(std::make_pair(vec2(-100, 1600), WATER_NORMAL));
		enemies.push_back(std::make_pair(vec2(200, 1600), WATER_NORMAL));

		// Right hallway
		terrains.push_back(std::make_pair(vec4(500, 1100, 200, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(500, 1375, 400, default_south_height), SOUTH_STATIONARY));
		floors.push_back(vec4(500, 1100, 400, 300));

		// Healthpack Area for Right hallway
		terrains.push_back(std::make_pair(vec4(700, 900, 200, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(675, 900, default_side_width, 200), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(900, 900, default_side_width, 500), SIDE_STATIONARY));
		floors.push_back(vec4(700, 1000, 200, 200));
		health_packs_pos.push_back(vec2(800, 1100));
		
		// Right Hallway from Spawn
		terrains.push_back(std::make_pair(vec4(1500, 200, 700, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1500, 500, 700, default_south_height), SOUTH_STATIONARY));
		floors.push_back(vec4(1500, 300, 700, 200));

		// Top Right Room
		terrains.push_back(std::make_pair(vec4(2200, -200, 1000, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2175, -200, default_side_width, 400), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2175, 500, default_side_width, 1000), SIDE_STATIONARY)); // extends down
		terrains.push_back(std::make_pair(vec4(2400, 1075, 800, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(3200, -200, default_side_width, 1300), SIDE_STATIONARY));

		enemies.push_back(std::make_pair(vec2(3000, 100), FIRE_NORMAL));
		enemies.push_back(std::make_pair(vec2(3000, 300), FIRE_NORMAL));
		enemies.push_back(std::make_pair(vec2(3000, 500), FIRE_NORMAL));
		enemies.push_back(std::make_pair(vec2(3000, 700), FIRE_NORMAL));
		enemies.push_back(std::make_pair(vec2(3000, 900), FIRE_NORMAL));

		floors.push_back(vec4(2200, -200, 1000, 1300));

		// Hallway down from Top Right Room
		terrains.push_back(std::make_pair(vec4(2400, 1100, default_side_width, 400), SIDE_STATIONARY));
		floors.push_back(vec4(2200, 1100, 200, 1800)); // extends down
		terrains.push_back(std::make_pair(vec4(2400, 1700, default_side_width, 1100), SIDE_STATIONARY)); // extends down

		// Weird Maze Thing with Health pack
		terrains.push_back(std::make_pair(vec4(2175, 1700, default_side_width, 400), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1600, 1400, 600, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1800, 1675, 400, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1575, 1400, default_side_width, 900), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1800, 1700, default_side_width, 400), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1800, 2000, 400, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1600, 2275, 600, default_south_height), SOUTH_STATIONARY));

		floors.push_back(vec4(1600, 1500, 600, 200));
		floors.push_back(vec4(1600, 1700, 200, 600));
		floors.push_back(vec4(1800, 2100, 400, 200));

		health_packs_pos.push_back(vec2(1710, 2175));

		// Hallway from maze to small right room
		terrains.push_back(std::make_pair(vec4(2400, 1400, 1000, default_north_height), NORTH_STATIONARY)); // extends right
		terrains.push_back(std::make_pair(vec4(2400, 1675, 400, default_south_height), SOUTH_STATIONARY));
		
		floors.push_back(vec4(2400, 1500, 400, 200));

		// Small right room
		terrains.push_back(std::make_pair(vec4(3400, 1400, default_side_width, 800), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2775, 1700, default_side_width, 500), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2800, 2175, 600, default_south_height), SOUTH_STATIONARY));

		enemies.push_back(std::make_pair(vec2(3000, 2000), WATER_NORMAL));

		floors.push_back(vec4(2800, 1500, 600, 700));

		// Hallway from maze down to bottom right room
		terrains.push_back(std::make_pair(vec4(2175, 2300, default_side_width, 500), SIDE_STATIONARY));

		this->player_starting_pos = vec2(2300, 3500);

		// Bottom right room
		terrains.push_back(std::make_pair(vec4(2900, 2800, default_side_width, 1200), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1700, 2800, 500, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2400, 2800, 500, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1675, 2800, default_side_width, 500), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1675, 3500, default_side_width, 500), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1700, 3975, 500, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2400, 3975, 500, default_south_height), SOUTH_STATIONARY));

		floors.push_back(vec4(1700, 2900, 1200, 1100));

		// mini room below Bottom right room with health pack
		terrains.push_back(std::make_pair(vec4(2175, 4000, default_side_width, 300), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2400, 4000, default_side_width, 300), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2200, 4275, 200, default_south_height), SOUTH_STATIONARY));

		enemies.push_back(std::make_pair(vec2(1800, 3800), EARTH_NORMAL));
		enemies.push_back(std::make_pair(vec2(2100, 3800), EARTH_NORMAL));
		enemies.push_back(std::make_pair(vec2(2400, 3800), EARTH_NORMAL));
		enemies.push_back(std::make_pair(vec2(2700, 3800), EARTH_NORMAL));

		floors.push_back(vec4(2200, 4000, 200, 300));

		health_packs_pos.push_back(vec2(2300, 4150));

		break;

	case WATER_BOSS:
		floors.push_back(vec4(25, 25, 2700, 1375));

		this->player_starting_pos = vec2(800, 650);
		this->exit_door_pos = NULL_POS;

		terrains.push_back(std::make_pair(vec4(25, 0, 2700, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 1375, 2700, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 1400), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2725, 0, default_side_width, 1400), SIDE_STATIONARY));

		bosses.push_back(std::make_pair(vec2(1400, 700), WATER_HIGH_DAMAGE));
		break;

	case FINAL_BOSS: // actual final boss (the reaper dude)
		floors.push_back(vec4(25, 25, 2700, 1375));

		this->player_starting_pos = vec2(800, 650);
		this->exit_door_pos = NULL_POS;

		terrains.push_back(std::make_pair(vec4(25, 0, 2700, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 1375, 2700, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 1400), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2725, 0, default_side_width, 1400), SIDE_STATIONARY));

		bosses.push_back(std::make_pair(vec2(1400, 700), COMBO_HIGH_DAMAGE));
		break;

	case POWER_UP:
		floors.push_back(vec4(25, 30, 1300, 650));
		
		this->player_starting_pos = vec2(500, 500);
		this->exit_door_pos = vec2(1200, 575);

		texts.push_back("Shoot the mystery boxes to power up!");
		text_attrs.push_back({ 0.f,150.f,1.0f,1.0f,1.0f,0.f });
		texts.push_back("Choose wisely... you can only keep one!");
		text_attrs.push_back({ 0.f,100.f,1.0f,1.0f,1.0f,0.f });

		terrains.push_back(std::make_pair(vec4(25, 0, 1300, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 675, 1300, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 700), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1325, 0, default_side_width, 700), SIDE_STATIONARY));
		break;

	default:
		printf("no level provided\n");
		break;
	}

	return true;
}