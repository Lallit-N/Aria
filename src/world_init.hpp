#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

// hard coded to the textures
const float HEALTH_BAR_HEIGHT = 0.3f * 33.f;
const float HEALTH_BAR_WIDTH = 0.3f * 369.f;

// the player
Entity createAria(RenderSystem* renderer, vec2 pos);
Entity createProjectile(RenderSystem* renderer, vec2 pos, vec2 vel, ElementType elementType);
// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size);

// creates a terrain with fixed size
Entity createTerrain(RenderSystem* renderer, vec2 pos, vec2 size);

// creates an exit door
Entity createExitDoor(RenderSystem* renderer, vec2 pos);

Entity createText(std::string in_text, vec2 pos, float scale, vec3 color);

// mock enemy TODO: change enemy implementation
Entity createEnemy(RenderSystem* renderer, vec2 pos, ElementType enemyType);

// creates a health bar associated with an owner entity
Entity createHealthBar(RenderSystem* renderer, Entity &owner_entity);

Entity createFloor(RenderSystem* renderer, vec2 pos);

// test
Entity createTestSalmon(RenderSystem* renderer, vec2 pos);

