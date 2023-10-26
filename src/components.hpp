#pragma once
#include "common.hpp"
#include <vector>
#include <unordered_map>
#include "../ext/stb_image/stb_image.h"

// Aria component
struct Player
{

};

// Enemy component
struct Enemy
{
	float damage = 10.f;
	float movementTimer = 3000.f;
	float stamina = 0.5f;
};

// Terrain
struct Terrain
{

};

// Exit door
struct ExitDoor
{

};

// All data relevant to the resources of entities
struct Resources
{
	float health = 100.f;
	float mana = 100.f;
};

// Structure to store projectile entities
struct Projectiles
{
	float damage = 10.f;
};

// All data relevant to the position of entities
struct Position {
	vec2 position = { 0.f, 0.f };
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
	FLOOR = TURTLE + 1,
	TEXTURE_COUNT = FLOOR + 1,
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
	EFFECT_COUNT = EXIT_DOOR + 1
};
const int effect_count = (int)EFFECT_ASSET_ID::EFFECT_COUNT;

enum class GEOMETRY_BUFFER_ID {
	ARIA = 0,
	SALMON = ARIA + 1,
	SPRITE = SALMON + 1,
	TURTLE = SPRITE + 1,
	DEBUG_LINE = SPRITE + 1,
	SCREEN_TRIANGLE = DEBUG_LINE + 1,
	TERRAIN = SCREEN_TRIANGLE + 1,
	EXIT_DOOR = TERRAIN + 1,
	GEOMETRY_COUNT = EXIT_DOOR + 1
};
const int geometry_count = (int)GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;

struct RenderRequest {
	TEXTURE_ASSET_ID used_texture = TEXTURE_ASSET_ID::TEXTURE_COUNT;
	EFFECT_ASSET_ID used_effect = EFFECT_ASSET_ID::EFFECT_COUNT;
	GEOMETRY_BUFFER_ID used_geometry = GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;
};

