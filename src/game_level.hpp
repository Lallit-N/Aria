#include <components.hpp>
#include <common.hpp> 

// Class that describes the current configurations of the current game level.
// This includes things like the player's starting position, the enemies, the terrain, etc.
// We don't have access to the renderer until the renderer system runs so we should only be defining
// the attributes of the enemies and terrain
const int TERRAIN_ATTRIBUTES = 4;
const int ENEMY_ATTRIBUTES = 6;
const int TEXT_ATTRIBUTES = 6;

class GameLevel
{
public:
	uint curr_level;
	vec2 player_starting_pos;
	vec2 exit_door_pos;

	std::vector<std::string> texts;

	// pos_x, pos_y, font_size, r, g, b
	std::vector<std::array<float, TEXT_ATTRIBUTES>> text_attrs;

	// pos_x, pos_y
	std::vector<vec2> floor_pos;

	// pos_x, pos_y, scale_x, scale_y
	std::vector<vec4> terrains_attr;

	// pos_x, pos_y, vel_x, vel_y, scale_x, scale_y
	std::vector<std::array<float, ENEMY_ATTRIBUTES>> enemies_attr;

	bool init(uint level);

	uint getCurrLevel() {
		return curr_level;
	}

	vec2& getPlayerStartingPos() {
		return player_starting_pos;
	}

	vec2& getExitDoorPos() {
		return exit_door_pos;
	}

	std::vector<std::string>& getTexts() {
		return texts;
	}

	std::vector<std::array<float, TEXT_ATTRIBUTES>>& getTextAttrs() {
		return text_attrs;
	}

	std::vector<vec2>& getFloorPos() {
		return floor_pos;
	}

	std::vector<vec4>& getTerrains() {
		return terrains_attr;
	}

	std::vector<std::array<float,ENEMY_ATTRIBUTES>>& getEnemies() {
		return enemies_attr;
	}
};