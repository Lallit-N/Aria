#include "game_level.hpp" 
#include <random>
#include <tiny_ecs_registry.hpp>

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

const double getRandomSpeed() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<double> distribution(75, 100);
	std::uniform_int_distribution<int> int_distribution(0, 1);

	return distribution(gen) * (int_distribution(gen) * 2 - 1);
}

bool GameLevel::init(uint level) {
	printf("Initializing game for level: %i\n", level);
	if (level < 0) {
		return false;
	}
	this->curr_level = level;
	this->is_cutscene = false;
	this->is_boss_level = false;

	std::vector<vec2>& health_packs_pos = this->health_packs_pos;
	std::vector<std::string>& texts = this->texts;
	std::vector<std::array<float, TEXT_ATTRIBUTES>>& text_attrs = this->text_attrs;
	std::vector<std::array< vec2, OBSTACLE_ATTRIBUTES>>& obstacles = this->obstacle_attrs;
	std::vector<vec4>& floors = this->floor_attrs;
	std::vector<std::pair<vec4, Terrain>>& terrains = this->terrains_attr;
	std::vector<std::pair<vec2, Enemy>>& enemies = this->enemies_attr;
	std::vector<std::pair<vec2, Enemy>>& bosses = this->bosses_attr;
	std::vector<std::pair<vec2, LostSoul>>& lost_souls = this->lost_souls_attr;

	health_packs_pos.clear();
	texts.clear();
	text_attrs.clear();
	floors.clear();
	terrains.clear();
	enemies.clear();
	obstacles.clear();
	bosses.clear();
	lost_souls.clear();

	this->hasEnemies = false;
	this->power_up_next_level = false;

	switch (level) {
	case CUTSCENE_1:
		texts.push_back("Press SPACE to skip");
		text_attrs.push_back({ 0.f,20.f,0.8f,1.0f,1.0f,0.f });

		this->is_cutscene = true;
		floors.push_back(vec4(25, 25, 10000, 400));

		this->player_starting_pos = vec2(50, 300);
		this->exit_door_pos = vec2(9300, 200);
		this->cutscene_player_velocity = { 300.f,0.f };

		lost_souls.push_back({ vec2(5800, 300), LostSoul() });

		terrains.push_back(std::make_pair(vec4(25, 0, 10000, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 400, 10000, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 425), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(10025, 0, default_side_width, 425), SIDE_STATIONARY));
		break;

	case TUTORIAL:
		floors.push_back(vec4(25, 30, 1300, 650));

		this->player_starting_pos = vec2(200, 200);
		this->exit_door_pos = vec2(1225, 580);

		health_packs_pos.push_back(vec2(400, 300));
		health_packs_pos.push_back(vec2(600, 400));
		health_packs_pos.push_back(vec2(800, 500));
		health_packs_pos.push_back(vec2(1000, 600));

		texts.push_back("ESC to pause");
		text_attrs.push_back({ 0.f,260.f,0.8f,1.0f,1.0f,0.f });
		texts.push_back("Use WASD to move around");
		text_attrs.push_back({ 0.f,220.f,0.8f,1.0f,1.0f,0.f });
		texts.push_back("Use M1 button to shoot");
		text_attrs.push_back({ 0.f,180.f,0.8f,1.0f,1.0f,0.f });
		texts.push_back("Use 1,2,3,4 to cycle through elements");
		text_attrs.push_back({ 0.f,140.f,0.8f,1.0f,1.0f,0.f });		
		texts.push_back("and also scroll-wheel and M2");
		text_attrs.push_back({ 0.f,100.f,0.8f,1.0f,1.0f,0.f });
		texts.push_back("Collect health packs to heal!");
		text_attrs.push_back({ 0.f,60.f,0.8f,1.0f,1.0f,0.f });
		texts.push_back("Move to the exit door when ready");
		text_attrs.push_back({ 0.f,20.f,0.8f,1.0f,1.0f,0.f });


		terrains.push_back(std::make_pair(vec4(25, 0, 1300, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 675, 1300, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 700), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1325, 0, default_side_width, 700), SIDE_STATIONARY));
		break;

	case TUTORIAL_2:
		floors.push_back(vec4(25, 30, 1300, 650));

		this->player_starting_pos = vec2(650, 300);
		this->exit_door_pos = vec2(1225, 580);
		this->power_up_next_level = true;

		texts.push_back("Enemies are weak to certain elements");
		text_attrs.push_back({ 0,220.f,0.8f,1.0f,1.0f,0.f });
		texts.push_back("Fire is weak to Water, Water is weak to Lightning");
		text_attrs.push_back({ 0.f,180.f,0.8f,1.0f,1.0f,0.f });
		texts.push_back("Lightning is weak to Earth, Earth is weak to Fire");
		text_attrs.push_back({ 0.f,140.f,0.8f,1.0f,1.0f,0.f });
		texts.push_back("Same elements will heal enemies");
		text_attrs.push_back({ 0.f,100.f,0.8f,1.0f,1.0f,0.f });
		texts.push_back("Try it out!");
		text_attrs.push_back({ 0.f,60.f,0.8f,1.0f,1.0f,0.f });		
		texts.push_back("Make sure to kill all enemies in the level");
		text_attrs.push_back({ 0.f,20.f,0.8f,1.0f,1.0f,0.f });

		enemies.push_back(std::make_pair(vec2(200, 200), FIRE_NORMAL)); // last enemy must be fire
		enemies.push_back(std::make_pair(vec2(200, 550), WATER_NORMAL)); // last enemy must be fire
		enemies.push_back(std::make_pair(vec2(900, 200), LIGHTNING_NORMAL)); // last enemy must be fire
		enemies.push_back(std::make_pair(vec2(900, 550), EARTH_NORMAL)); // last enemy must be fire

		terrains.push_back(std::make_pair(vec4(25, 0, 1300, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 675, 1300, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 700), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1325, 0, default_side_width, 700), SIDE_STATIONARY));
		break;
	case LEVEL_1:
		this->player_starting_pos = vec2(200, 700);
		this->exit_door_pos = vec2(1450, 775);
		this->power_up_next_level = true;

		floors.push_back(vec4(25, 25, 1800, 875));

		// I think we should make a helper for pushing back into terrains vector, thoughts? - Leo
		terrains.push_back(std::make_pair(vec4(25, 0, 1800, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 875, 1800, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 900), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1825, 0, default_side_width, 900), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(350, 400, default_side_width, 475), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(750, 0, default_side_width, 475), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1150, 400, default_side_width, 475), SIDE_STATIONARY));

		this->hasEnemies = true;
		enemies.push_back(std::make_pair(vec2(350, 250), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(700, 800), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(950, 800), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(1100, 250), FIRE_NORMAL)); // last enemy must be fire
		break;

	case FIRE_BOSS:
		this->is_boss_level = true;
		this->life_orb_piece = NULL;

		floors.push_back(vec4(25, 25, 2700, 1375));

		this->player_starting_pos = vec2(800, 650);
		this->exit_door_pos = NULL_POS;

		terrains.push_back(std::make_pair(vec4(25, 0, 2700, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 1375, 2700, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 1400), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2725, 0, default_side_width, 1400), SIDE_STATIONARY));

		bosses.push_back(std::make_pair(vec2(1400, 700), FIRE_HIGH_DAMAGE));
		break;

	case CUTSCENE_2:
		this->is_cutscene = true;


		terrains.push_back(std::make_pair(vec4(25, 0, 3000, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 400, 2900, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 0, default_side_width, 400), SIDE_STATIONARY));
		floors.push_back(vec4(25, 25, 3000, 400));

		terrains.push_back(std::make_pair(vec4(2900, 400, default_side_width, 2200), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(3000, 0, default_side_width, 1000), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(3000, 1200, default_side_width, 1600), SIDE_STATIONARY));

		floors.push_back(vec4(2900, 400, 100, 2400));

		//extra room
		terrains.push_back(std::make_pair(vec4(3000, 1000, 1000, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(3000, 1200, 1000, default_south_height), SOUTH_STATIONARY));
		floors.push_back(vec4(3000, 1000, 1000, 200));


		terrains.push_back(std::make_pair(vec4(25, 2500, 2900, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 2800, 3000, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 2500, default_side_width, 300), SIDE_STATIONARY));
		floors.push_back(vec4(25, 2500, 3000, 300));

		this->player_starting_pos = vec2(25, 300);
		//POSSIBLE MEMORY LEAK IN LOSTSOUL()
		lost_souls.push_back({ vec2(200, 200), LostSoul() });

		this->exit_door_pos = vec2(125, 2650);
		this->cutscene_player_velocity = { 220,0.f };

		// add a bunch of enemies outside the map walls
		// above top
		for (int i = 25; i <= 2800; i += 100) {
			Enemy enemy = getRandomNormalEnemy();
			enemy.isAggravated = false;
			enemies.push_back(std::make_pair(vec2(i, -50), enemy));
		}
		// under top
		for (int i = 25; i <= 2800; i += 100) {
			Enemy enemy = getRandomNormalEnemy();
			enemy.isAggravated = false;
			enemies.push_back(std::make_pair(vec2(i, 500), enemy));
		}
		// far right
		for (int i = 0; i <= 900; i += 100) {
			Enemy enemy = getRandomNormalEnemy();
			enemy.isAggravated = false;
			enemies.push_back(std::make_pair(vec2(3050, i), enemy));
		}
		for (int i = 1300; i <= 2800; i += 100) {
			Enemy enemy = getRandomNormalEnemy();
			enemy.isAggravated = false;
			enemies.push_back(std::make_pair(vec2(3050, i), enemy));
		}
		// inside right
		for (int i = 700; i <= 2600; i += 100) {
			Enemy enemy = getRandomNormalEnemy();
			enemy.isAggravated = false;
			enemies.push_back(std::make_pair(vec2(2850, i), enemy));
		}
		//// above bottom
		for (int i = 100; i <= 2000; i += 100) {
			Enemy enemy = getRandomNormalEnemy();
			enemy.isAggravated = false;
			enemies.push_back(std::make_pair(vec2(i, 2500), enemy));
		}
		// under bottom
		for (int i = 100; i <= 2000; i += 100) {
			Enemy enemy = getRandomNormalEnemy();
			enemy.isAggravated = false;
			enemies.push_back(std::make_pair(vec2(i, 2900), enemy));
		}

		break;

	case LEVEL_2:
		this->player_starting_pos = vec2(200, 200);
		this->exit_door_pos = vec2(3900, 1880);
		this->power_up_next_level = true;

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

		terrains.push_back(std::make_pair(vec4(3800, 700, 75, 75), GENERIC_MOVABLE_SLOW));
		terrains.push_back(std::make_pair(vec4(3600, 800, 75, 75), GENERIC_MOVABLE_SLOW));
		terrains.push_back(std::make_pair(vec4(3400, 900, 75, 75), GENERIC_MOVABLE_SLOW));

		this->hasEnemies = true;
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

	case EARTH_BOSS:
		this->is_boss_level = true;
		this->life_orb_piece = 1;
		floors.push_back(vec4(25, 25, 2700, 1375));

		this->player_starting_pos = vec2(800, 650);
		this->exit_door_pos = NULL_POS;

		terrains.push_back(std::make_pair(vec4(25, 0, 2700, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 1375, 2700, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 1400), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2725, 0, default_side_width, 1400), SIDE_STATIONARY));

		bosses.push_back(std::make_pair(vec2(1400, 700), EARTH_HIGH_DAMAGE));
		break;

	case CUTSCENE_3:
		this->is_cutscene = true;
		floors.push_back(vec4(25, 25, 700, 400));
		this->life_orb_piece = 1;

		this->player_starting_pos = vec2(562, 280);
		this->exit_door_pos = vec2(4800, 200);
		this->cutscene_player_velocity = { 0.f,0.f };

		lost_souls.push_back({ vec2(162, 280), LostSoul() });

		terrains.push_back(std::make_pair(vec4(25, 0, 700, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 400, 700, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 425), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(725, 0, default_side_width, 425), SIDE_STATIONARY));

		break;

	case LEVEL_3: // Same as level 1 but with moving walls
		this->player_starting_pos = vec2(1000, 400);
		this->exit_door_pos = vec2(-100, 3325);
		this->power_up_next_level = true;

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

		// Left hallway from Bottom right room
		terrains.push_back(std::make_pair(vec4(-200, 3200, 1900, default_north_height), NORTH_STATIONARY)); // extends
		terrains.push_back(std::make_pair(vec4(1100, 3500, 600, default_south_height), SOUTH_STATIONARY));

		floors.push_back(vec4(1100, 3300, 600, 200));

		// Small bottom room
		terrains.push_back(std::make_pair(vec4(1100, 3525, default_side_width, 400), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(275, 3525, default_side_width, 400), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(800, 3900, 300, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(300, 3900, 300, default_south_height), SOUTH_STATIONARY));

		enemies.push_back(std::make_pair(vec2(500, 3800), LIGHTNING_NORMAL));
		enemies.push_back(std::make_pair(vec2(700, 3800), LIGHTNING_NORMAL));

		floors.push_back(vec4(300, 3300, 800, 600));

		// Health pack in small bottom room
		terrains.push_back(std::make_pair(vec4(575, 3925, default_side_width, 200), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(800, 3925, default_side_width, 200), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(600, 4100, 200, default_south_height), SOUTH_STATIONARY));

		health_packs_pos.push_back(vec2(700, 4000));

		floors.push_back(vec4(600, 3900, 200, 200));

		// Hallway to the Portal room
		terrains.push_back(std::make_pair(vec4(-200, 3500, 500, default_south_height), SOUTH_STATIONARY));

		floors.push_back(vec4(-200, 3300, 500, 200));

		this->hasEnemies = true;

		// Portal room
		terrains.push_back(std::make_pair(vec4(-200, 3200, default_side_width, 300), SIDE_STATIONARY));
		break;

	case LIGHTNING_BOSS:
		this->is_boss_level = true;
		this->life_orb_piece = 2;
		floors.push_back(vec4(25, 25, 2700, 1375));

		this->player_starting_pos = vec2(800, 650);
		this->exit_door_pos = NULL_POS;

		terrains.push_back(std::make_pair(vec4(25, 0, 2700, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 1375, 2700, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 1400), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2725, 0, default_side_width, 1400), SIDE_STATIONARY));

		bosses.push_back(std::make_pair(vec2(1400, 700), LIGHTNING_HIGH_DAMAGE));
		break;

	case CUTSCENE_4:
		this->is_cutscene = true;
		floors.push_back(vec4(25, 25, 1500, 400));

		this->player_starting_pos = vec2(50, 300);
		this->exit_door_pos = vec2(1300, 200);
		this->cutscene_player_velocity = { 300.f,0.f };

		lost_souls.push_back({ vec2(200, 300), LostSoul() });

		terrains.push_back(std::make_pair(vec4(25, 0, 1500, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 400, 1500, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 425), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1525, 0, default_side_width, 425), SIDE_STATIONARY));

		break;

	case LEVEL_4:
		this->player_starting_pos = vec2(100, 3750);
		this->power_up_next_level = true;

		texts.push_back("Don't get haunted by the ghosts!");
		text_attrs.push_back({ 0.f,50.f,1.0f,1.0f,1.0f,0.f });

		floors.push_back(vec4(1500, 800, 500, 3000));
		floors.push_back(vec4(0, 500, 2000, 300));
		floors.push_back(vec4(0, 3575, 2000, 450));

		// Outer maze walls
		terrains.push_back(std::make_pair(vec4(0, 475, 2000, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 4000, 2000, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(-25, 475, default_side_width, 350), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(-25, 3475, default_side_width, 550), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2000, 475, default_side_width, 3550), SIDE_STATIONARY));

		// Beginning obstacles
		obstacles.push_back({ vec2(400, 3650), vec2(80,80), vec2(getRandomSpeed(),getRandomSpeed()) });
		obstacles.push_back({ vec2(600, 3800), vec2(80,80), vec2(getRandomSpeed(),getRandomSpeed()) });
		obstacles.push_back({ vec2(800, 3650), vec2(80,80), vec2(getRandomSpeed(),getRandomSpeed()) });
		obstacles.push_back({ vec2(1000, 3800), vec2(80,80), vec2(getRandomSpeed(),getRandomSpeed()) });

		// Bottom right L
		terrains.push_back(std::make_pair(vec4(0, 3475, 1475, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1475, 800, default_side_width, 3100), SIDE_STATIONARY));

		// Enemies in right hallway
		enemies.push_back(std::make_pair(vec2(1775, 2100), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(1655, 2600), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(1875, 3000), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(1700, 3600), getRandomNormalEnemy()));

		// North walls in right hallway
		terrains.push_back(std::make_pair(vec4(1800, 1900, 200, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1500, 1900, 200, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1500, 2200, 400, default_north_height), NORTH_STATIONARY));
		obstacles.push_back({ vec2(1750, 2450), vec2(150, 150), vec2(75.f, 75.f) });
		terrains.push_back(std::make_pair(vec4(1600, 2700, 400, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1500, 3000, 300, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1600, 3300, 400, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1500, 3600, 200, default_north_height), NORTH_STATIONARY));

		// Obstacle ghost timing
		terrains.push_back(std::make_pair(vec4(1675, 1600, default_side_width, 300), SIDE_STATIONARY));
		obstacles.push_back({ vec2(1675, 1565), vec2(70, 70), vec2(150.f, 0.f) });
		terrains.push_back(std::make_pair(vec4(1800, 1600, default_side_width, 300), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1675, 1320, default_side_width, 200), SIDE_STATIONARY));
		obstacles.push_back({ vec2(1700, 1285), vec2(70, 70), vec2(175.f, 0.f) });
		terrains.push_back(std::make_pair(vec4(1800, 1320, default_side_width, 200), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1675, 1040, default_side_width, 200), SIDE_STATIONARY));
		obstacles.push_back({ vec2(1725, 1005), vec2(70, 70), vec2(200.f, 0.f) });
		terrains.push_back(std::make_pair(vec4(1800, 1040, default_side_width, 200), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1675, 860, default_side_width, 100), SIDE_STATIONARY));
		obstacles.push_back({ vec2(1750, 825), vec2(70, 70), vec2(250.f, 0.f) });
		terrains.push_back(std::make_pair(vec4(1800, 860, default_side_width, 100), SIDE_STATIONARY));

		// Health packs!
		health_packs_pos.push_back(vec2(1750, 2450));
		health_packs_pos.push_back(vec2(1750, 2900));

		// Top inner wall
		terrains.push_back(std::make_pair(vec4(0, 800, 1475, default_south_height), SOUTH_STATIONARY));

		// Enemies top left corner
		this->hasEnemies = true;
		enemies.push_back(std::make_pair(vec2(1300, 650), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(700, 650), getRandomNormalEnemy()));
		enemies.push_back(std::make_pair(vec2(350, 650), getRandomNormalEnemy()));

		this->exit_door_pos = vec2(50, 675);
		break;
	case WATER_BOSS:
		this->is_boss_level = true;
		this->life_orb_piece = 3;
		floors.push_back(vec4(25, 25, 2700, 1375));

		this->player_starting_pos = vec2(800, 650);
		this->exit_door_pos = NULL_POS;

		terrains.push_back(std::make_pair(vec4(25, 0, 2700, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 1375, 2700, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 1400), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2725, 0, default_side_width, 1400), SIDE_STATIONARY));

		bosses.push_back(std::make_pair(vec2(1400, 700), WATER_HIGH_DAMAGE));
		break;

	case CUTSCENE_5:
		this->is_cutscene = true;
		floors.push_back(vec4(-400, 0, 800, 300));
		floors.push_back(vec4(400, -200, 1000, 700));

		this->player_starting_pos = vec2(-200, 200);
		this->exit_door_pos = vec2(4800, 200);
		this->cutscene_player_velocity = { 0.f,0.f };

		this->life_orb_piece = 0; // full piece

		lost_souls.push_back({ vec2(0, 200), LostSoul() });

		terrains.push_back(std::make_pair(vec4(-400, 0, 825, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(-400, 300, 800, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(-400, 0, default_side_width, 300), SIDE_STATIONARY));

		terrains.push_back(std::make_pair(vec4(400, -200, 1000, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 300, 400, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(400, 500, 1000, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(400, -200, default_side_width, 200), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(400, 300, default_side_width, 200), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(1400, -200, default_side_width, 725), SIDE_STATIONARY));


		//TIMER 
		obstacles.push_back({ vec2(0, -300), vec2(30,30), vec2(10.f,0.f) });



		break;

	case FINAL_BOSS: // actual final boss (the reaper dude)
		floors.push_back(vec4(25, 25, 2700, 1375));

		this->player_starting_pos = vec2(800, 650);
		this->exit_door_pos = NULL_POS;

		terrains.push_back(std::make_pair(vec4(25, 0, 2700, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 1375, 2700, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 1400), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(2725, 0, default_side_width, 1400), SIDE_STATIONARY));

		bosses.push_back(std::make_pair(vec2(1400, 700), FINAL_BOSS_ATTRS));
		break;

	case CUTSCENE_6:
		this->is_cutscene = true;

		floors.push_back(vec4(25, 25, 12000, 400));

		this->player_starting_pos = vec2(50, 300);
		this->exit_door_pos = vec2(2300, 1000); // render outside for now
		this->cutscene_player_velocity = { 300.f,0.f };
		this->life_orb_piece = 0; // full piece

		terrains.push_back(std::make_pair(vec4(25, 0, 12000, default_north_height), NORTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(25, 400, 12000, default_south_height), SOUTH_STATIONARY));
		terrains.push_back(std::make_pair(vec4(0, 0, default_side_width, 425), SIDE_STATIONARY));
		terrains.push_back(std::make_pair(vec4(12025, 0, default_side_width, 425), SIDE_STATIONARY));
		lost_souls.push_back({ vec2(115, 200), LostSoul() });

		
		//TIMER 
		obstacles.push_back({ vec2(0, -300), vec2(30,30), vec2(10.f,0.f) });

		break;

	case THE_END:
		this->is_cutscene = true;
		this->player_starting_pos = vec2(600, 400);
		this->exit_door_pos = NULL_POS;

		texts.push_back("THE END");
		text_attrs.push_back({ 500.f,307.f,1.0f,1.0f,1.0f,0.f });

		texts.push_back("Press ESC to restart");
		text_attrs.push_back({ 0.f,20.f,0.8f,1.0f,1.0f,0.f });

		break;
	case POWER_UP:
		floors.push_back(vec4(25, 30, 1300, 650));

		this->player_starting_pos = vec2(500, 500);
		this->exit_door_pos = vec2(1228, 580);

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