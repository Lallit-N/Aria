#pragma once
#include "common.hpp"
#include <vector>
#include <map>
#include <unordered_map>
#include "../ext/stb_image/stb_image.h"
using namespace std;

// Aria component
struct Player
{
};
// All data relevant to elements and weaknesses
enum ElementType {
	WATER=0,
	FIRE=1,
	EARTH= 2,
	LIGHTNING = 3
};

// Enemy component
struct Enemy
{
	float damage = 10.f;
	float movementTimer = 3000.f;
	float stamina = 0.5f;
	float mana = 1.f;
	ElementType type = ElementType::FIRE; // By default, an enemy is of fire type
};

// all data relevant to the player's power ups
struct PowerUp {
	bool fasterMovement;
	bool increasedDamage[4]; // array size == number of element types
	bool tripleShot[4];
	bool bounceOffWalls[4];
};

// all data related to power up block in the power up level
struct PowerUpBlock {
	string powerUpText;
	bool* powerUpToggle;
};

// Terrain
struct Terrain
{
	bool moveable = false;
};

// Exit door
struct ExitDoor
{

};

// Text component
struct Text
{
	std::string text;
	vec3 color;
};

// All data relevant to the resources of entities
struct Resources
{
	float maxHealth = 100.f;
	float currentHealth = 100.f;
	float maxMana = 10.f;
	float currentMana = 10.f;
	Entity healthBar;
	Entity manaBar;
};

struct HealthBar
{
	Entity owner;
	float y_offset = -60.f;
};

struct ManaBar
{
	Entity owner;
	float y_offset = -75.f;
};

// Structure to store projectile entities
struct Projectile
{
	float damage = 10.f;
	ElementType type;
	bool hostile = false;
	int bounces;
};

struct CharacterProjectileType
{
	ElementType projectileType = ElementType::WATER; //By default, the characters projectile type is water
};

// All data relevant to the position of entities
struct Position {
	vec2 position = { 0.f, 0.f };
	float angle = 0.f;
	vec2 scale = { 10.f, 10.f };
	vec2 prev_position = { 0.f, 0.f };
};

// Data relevant to velocity of entities
struct Velocity {
	vec2 velocity = { 0.f, 0.f };
};


// Data relevant to direction of entities
typedef enum {
	N,
	NE,
	E,
	SE,
	S,
	SW,
	W,
	NW,
	NONE
} DIRECTION;

struct Direction {
	DIRECTION direction;
};

// Stucture to store collision information
struct Collision
{
	// Note, the first object is stored in the ECS container.entities
	Entity other_entity; // the second object involved in the collision
	vec2 displacement;
	Collision(Entity& other_entity, vec2 displacement) { 
		this->other_entity = other_entity;
		this->displacement = displacement;
	};
};

// Component container that marks an entity as being collidable
// Will be: players, enemies, terrain, projectiles, etc.
struct Collidable
{

};

// Data structure for toggling debug mode
struct Debug {
	bool in_debug_mode = 0;
	bool in_freeze_mode = 0;
};
extern Debug debugging;

// Sets the brightness of the screen
struct ScreenState
{
	float screen_darken_factor = -1;
	bool apply_spotlight = false;
	float spotlight_radius = 0.0f;
};

// A struct to refer to debugging graphics in the ECS
struct DebugComponent
{
	// Note, an empty struct has size 1
};

// A timer that will be associated to an entity having an invulnerability period to damage
struct InvulnerableTimer
{
	float timer_ms = 1000.f;
};

// A timer that will be associated to an entity dying
struct DeathTimer
{
	float timer_ms = 3000.f;
};

// Timer that signifies level change
struct WinTimer
{
	float timer_ms = 1500.f;
	bool changedLevel = false;
};

// Single Vertex Buffer element for non-textured meshes (coloured.vs.glsl & salmon.vs.glsl)
struct ColoredVertex
{
	vec3 position;
	vec3 color;
};

// Single Vertex Buffer element for textured sprites (textured.vs.glsl)
struct TexturedVertex
{
	vec3 position;
	vec2 texcoord;
};

// Mesh datastructure for storing vertex and index buffers
struct Mesh
{
	static bool loadFromOBJFile(std::string obj_path, std::vector<ColoredVertex>& out_vertices, std::vector<uint16_t>& out_vertex_indices, vec2& out_size);
	vec2 original_size = {1,1};
	std::vector<ColoredVertex> vertices;
	std::vector<uint16_t> vertex_indices;
};

struct Animation
{
	std::vector<vec2> states; // list of pairs (first_frame, last_frame)
	int state = 0;
	int frame = 0;
	int num_rows;
	int num_cols;
	bool is_animating = true;
	bool rainbow_enabled = false;
	static vec2 getFrameSizeInTexcoords(int num_cols, int num_rows);
	static int getNumFrames(int num_cols, int num_rows);
	static int getColumn(int frame, int num_cols);
	static int getRow(int frame, int num_cols);
};

/**
 * The following enumerators represent global identifiers refering to graphic
 * assets. For example TEXTURE_ASSET_ID are the identifiers of each texture
 * currently supported by the system.
 *
 * So, instead of referring to a game asset directly, the game logic just
 * uses these enumerators and the RenderRequest struct to inform the renderer
 * how to structure the next draw command.
 *
 * There are 2 reasons for this:
 *
 * First, game assets such as textures and meshes are large and should not be
 * copied around as this wastes memory and runtime. Thus separating the data
 * from its representation makes the system faster.
 *
 * Second, it is good practice to decouple the game logic from the render logic.
 * Imagine, for example, changing from OpenGL to Vulkan, if the game logic
 * depends on OpenGL semantics it will be much harder to do the switch than if
 * the renderer encapsulates all asset data and the game logic is agnostic to it.
 *
 * The final value in each enumeration is both a way to keep track of how many
 * enums there are, and as a default value to represent uninitialized fields.
 */

enum class TEXTURE_ASSET_ID {
	FISH = 0,
	LANDSCAPE = FISH + 1,
	TURTLE = LANDSCAPE + 1,
	FIRE_ENEMY = TURTLE + 1,
	WATER_PROJECTILE = FIRE_ENEMY + 1,
	FIRE_PROJECTILE = WATER_PROJECTILE + 1,
	EARTH_PROJECTILE = FIRE_PROJECTILE + 1,
	LIGHTNING_PROJECTILE = EARTH_PROJECTILE + 1,
	WATER_PROJECTILE_SHEET = LIGHTNING_PROJECTILE + 1,
	FIRE_PROJECTILE_SHEET = WATER_PROJECTILE_SHEET + 1,
	FLOOR = FIRE_PROJECTILE_SHEET + 1,
	HEALTH_BAR = FLOOR + 1,
	MANA_BAR = HEALTH_BAR + 1,
	POWER_UP_BLOCK = MANA_BAR + 1,
	TEXTURE_COUNT = POWER_UP_BLOCK + 1
};
const int texture_count = (int)TEXTURE_ASSET_ID::TEXTURE_COUNT;

enum class EFFECT_ASSET_ID {
	ARIA = 0,
	COLOURED = ARIA + 1,
	SALMON = COLOURED + 1,
	TEXTURED = SALMON + 1,
	WATER = TEXTURED + 1,
	TERRAIN = WATER + 1,
	EXIT_DOOR = TERRAIN + 1,
	RESOURCE_BAR = EXIT_DOOR + 1,
	TEXT_2D = RESOURCE_BAR + 1,
	ANIMATED = TEXT_2D + 1,
	EFFECT_COUNT = ANIMATED + 1
};
const int effect_count = (int)EFFECT_ASSET_ID::EFFECT_COUNT;

enum class GEOMETRY_BUFFER_ID {
	ARIA = 0,
	SALMON = ARIA + 1,
	SPRITE = SALMON + 1,
	DEBUG_LINE = SPRITE + 1,
	SCREEN_TRIANGLE = DEBUG_LINE + 1,
	TERRAIN = SCREEN_TRIANGLE + 1,
	EXIT_DOOR = TERRAIN + 1,
	TEXT_2D = EXIT_DOOR + 1,
	RESOURCE_BAR = TEXT_2D + 1,
	PROJECTILE = RESOURCE_BAR + 1,
	POWER_UP_BLOCK = PROJECTILE + 1,
	GEOMETRY_COUNT = POWER_UP_BLOCK + 1
};
const int geometry_count = (int)GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;

enum class SPRITE_SHEET_DATA_ID {
	NONE = 0,
	PROJECTILE = NONE + 1,
	POWER_UP_BLOCK = PROJECTILE + 1,
	SPRITE_SHEET_COUNT = POWER_UP_BLOCK + 1
};
const int sprite_sheet_count = (int)SPRITE_SHEET_DATA_ID::SPRITE_SHEET_COUNT;

struct RenderRequest {
	TEXTURE_ASSET_ID used_texture = TEXTURE_ASSET_ID::TEXTURE_COUNT;
	EFFECT_ASSET_ID used_effect = EFFECT_ASSET_ID::EFFECT_COUNT;
	GEOMETRY_BUFFER_ID used_geometry = GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;
};

// One for each sprite sheet to indicate the states
enum class POWER_UP_BLOCK_STATES {
	ACTIVE = 0,
	NUM_ROWS = ACTIVE + 1
};
const int num_states_power_up_block = (int)POWER_UP_BLOCK_STATES::NUM_ROWS;

enum class PROJECTILE_STATES {
	MOVING = 0,
	NUM_ROWS = MOVING + 1
};
const int num_states_projectile = (int)PROJECTILE_STATES::NUM_ROWS;
