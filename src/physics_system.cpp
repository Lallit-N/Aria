// internal
#include "physics_system.hpp"
#include "world_init.hpp"

// Returns the local bounding coordinates scaled by the current size of the entity
vec2 get_bounding_box(const Position& position)
{
	// abs is to avoid negative scale due to the facing direction.
	return { abs(position.scale.x), abs(position.scale.y) };
}

// This is a SUPER APPROXIMATE check that puts a circle around the bounding boxes and sees
// if the center point of either object is inside the other's bounding-box-circle. You can
// surely implement a more accurate detection
bool circleCollides(const Position& position1, const Position& position2)
{
	vec2 dp = position1.position - position2.position;
	float dist_squared = dot(dp,dp);
	const vec2 other_bonding_box = get_bounding_box(position1) / 2.f;
	const float other_r_squared = dot(other_bonding_box, other_bonding_box);
	const vec2 my_bonding_box = get_bounding_box(position2) / 2.f;
	const float my_r_squared = dot(my_bonding_box, my_bonding_box);
	const float r_squared = std::max(other_r_squared, my_r_squared);
	if (dist_squared < r_squared)
		return true;
	return false;
}

vec2 worldTransform(vec2 coords, Position& position) {
	// !!! TODO: add rotation
	return vec2(coords.x * position.scale.x + position.position.x, coords.y * position.scale.y + position.position.y);
}

// Reference: 
// https://github.com/OneLoneCoder/Javidx9/blob/master/PixelGameEngine/SmallerProjects/OneLoneCoder_PGE_PolygonCollisions1.cpp?fbclid=IwAR1e0EyRPtFFGmg1EuiiKU9JxBwOAFN42YA3LIvfm0GHspBbE1df43ZeCz8
void diagonalCollides(Entity& ent_i, Entity& ent_j)
{
	Entity* entity_i = &ent_i;
	Entity* entity_j = &ent_j;
	bool flag = false;

	for (int obj = 0; obj < 2; obj++) {
		if (obj == 1) {
			entity_i = &ent_j;
			entity_j = &ent_i;
		}
		std::vector<ColoredVertex> i_vertices = registry.meshPtrs.get(*entity_i)->vertices;
		std::vector<ColoredVertex> j_vertices = registry.meshPtrs.get(*entity_j)->vertices;
		// figure how to get vertices of things also made for textures

		Position& position_i = registry.positions.get(*entity_i);
		Position& position_j = registry.positions.get(*entity_j);

		for (uint i = 0; i < i_vertices.size(); i++)
		{
			vec2 i_line_start = position_i.position;
			// Will need to add rotation into transformation as well
			vec2 i_line_end = worldTransform(vec2(i_vertices[i].position.x, i_vertices[i].position.y), position_i);
			vec2 displacement = { 0, 0 };

			for (uint j = 0; j < j_vertices.size(); j++) {
				uint next_point = (j + 1) % j_vertices.size();
				vec2 j_line_start = worldTransform(vec2(j_vertices[j].position.x, j_vertices[j].position.y), position_j);
				vec2 j_line_end = worldTransform(vec2(j_vertices[next_point].position.x, j_vertices[next_point].position.y), position_j);

				// Line segment intersection
				float h = (j_line_end.x - j_line_start.x) * (i_line_start.y - i_line_end.y) - (i_line_start.x - i_line_end.x) * (j_line_end.y - j_line_start.y);
				float t = ((j_line_start.y - j_line_end.y) * (i_line_start.x - j_line_start.x) + (j_line_end.x - j_line_start.x) * (i_line_start.y - j_line_start.y)) / h;
				float r = ((i_line_start.y - i_line_end.y) * (i_line_start.x - j_line_start.x) + (i_line_end.x - i_line_start.x) * (i_line_start.y - j_line_start.y)) / h;

				if (t >= 0.0f && t < 1.0f && r >= 0.0f && r < 1.0f)
				{
					displacement.x += (1.0f - t) * (i_line_end.x - i_line_start.x);
					displacement.y += (1.0f - t) * (i_line_end.y - i_line_start.y);
					flag = true;
				}
			}
			if (flag) {
				// ent_i is obj = 0 meaning it penetrated ent_j, so displacement is negative so we pull back ent_i's position
				registry.collisions.emplace_with_duplicates(ent_i, ent_j, (obj == 0) ? -displacement : displacement);
				// ent_j is obj = 0 meaning it penetrated ent_i, so displacement is positive to push away ent_i
				registry.collisions.emplace_with_duplicates(ent_j, ent_i, (obj == 0) ? displacement : -displacement);
				return;
			}
		}
	}
	return;
}

// Function that checks if two entities collides using an axis-aligned bounding-box implementation.
// If there is a collision between two entities, add a Collision component to the Component container
// marking both entities involved in the collision and the direction which entity i hit entity j from.
bool AABBCollides(Entity& entity_i, Entity& entity_j)
{
	auto& position_container = registry.positions;
	Position& position_i = position_container.get(entity_i);
	Position& position_j = position_container.get(entity_j);

	float i_left = position_i.position.x - abs(position_i.scale.x) / 2;
	float i_right = position_i.position.x + abs(position_i.scale.x) / 2;
	float i_top = position_i.position.y - abs(position_i.scale.y) / 2;
	float i_bottom = position_i.position.y + abs(position_i.scale.y) / 2;
	float j_left = position_j.position.x - abs(position_j.scale.x) / 2;
	float j_right = position_j.position.x + abs(position_j.scale.x) / 2;
	float j_top = position_j.position.y - abs(position_j.scale.y) / 2;
	float j_bottom = position_j.position.y + abs(position_j.scale.y) / 2;

	if (i_left <= j_right && i_right >= j_left && i_bottom >= j_top && i_top <= j_bottom)
	{
		return true;
	}
	return false;
}

// Shouldn't care if terrain-terrain and exitDoor-terrain collisions happen
bool shouldIgnoreCollision(Entity& entity_i, Entity& entity_j) 
{
	if (registry.terrain.has(entity_i) && registry.terrain.has(entity_j)) {
		if (registry.terrain.get(entity_i).moveable || registry.terrain.get(entity_j).moveable) {
			return false;
		}
		return true;
	}
	if ((registry.terrain.has(entity_i) && registry.exitDoors.has(entity_j)) ||
		(registry.exitDoors.has(entity_i) && registry.terrain.has(entity_j))) {
		return true;
	}
	return false;
}

void updateShadows() {
	Entity player_entity = registry.players.entities[0];
	
	Position& light_source_pos = (registry.lifeOrbs.entities.size() > 0)
		? registry.positions.get(registry.lifeOrbs.entities[0])
		: registry.positions.get(player_entity); //

	for (uint i = 0; i < registry.shadows.entities.size(); i++) {
		Entity entity = registry.shadows.entities[i];
		Shadow& shadow = registry.shadows.get(entity);

		Entity owner_entity = shadow.owner;

		Position& shadow_pos = registry.positions.get(entity);
		if (!registry.positions.has(owner_entity)) {
			registry.remove_all_components_of(entity);
			continue;
		}
		Position& owner_pos = registry.positions.get(owner_entity);
		shadow.active = true;

		if (distance((shadow_pos.position / vec2(window_width_px, window_height_px)), 
			(light_source_pos.position / vec2(window_width_px, window_height_px))) > light_radius) {
			shadow.active = false;
		}

		shadow_pos.position = owner_pos.position;

		// M_PI / 2 is to make the shadow upright
		shadow_pos.angle = atan2(owner_pos.position.y - light_source_pos.position.y, owner_pos.position.x - light_source_pos.position.x) + M_PI/2;

		float max_dist = light_radius*std::max(window_width_px, window_height_px);

		float dist = abs(distance(owner_pos.position, light_source_pos.position));
		shadow_pos.scale = owner_pos.scale * (max_dist - dist) / max_dist;
		shadow_pos.scale.y *= 1.5;

		shadow_pos.position.x += cos(shadow_pos.angle - M_PI / 2) * (shadow_pos.scale.y / 2);
		shadow_pos.position.y += owner_pos.scale.y / 2 + shadow_pos.scale.y / 2 * sin(shadow_pos.angle - M_PI/2);
	}
}

void PhysicsSystem::step(float elapsed_ms)
{
	if (registry.deathTimers.entities.size() > 0) return;
	auto& velocity_container = registry.velocities;
	for (uint i = 0; i < velocity_container.size(); i++)
	{
		Entity entity = velocity_container.entities[i];
		if (!velocity_container.has(entity) || !registry.positions.has(entity)) continue;
		Velocity& velocity = velocity_container.get(entity);
		Position& position = registry.positions.get(entity);
		float step_seconds = elapsed_ms / 1000.f;
		position.prev_position = position.position;
		position.position[0] += step_seconds * velocity.velocity[0];
		position.position[1] += step_seconds * velocity.velocity[1];
	}

	// Update shadows
	updateShadows();

	// Check for collisions between things that are collidable
	auto& collidables_container = registry.collidables;
	for (uint i = 0; i < collidables_container.size(); i++) {
		Entity& entity_i = collidables_container.entities[i];
		for (uint j = i+1; j < collidables_container.size(); j++) {
			Entity& entity_j = collidables_container.entities[j];
			// Ignore terrain-terrain and terrain-exitDoor collision
			if (shouldIgnoreCollision(entity_i, entity_j)) continue;
			// Broad phase of collision check
			if (AABBCollides(entity_i, entity_j)) {
				// Narrow phase of collision check
				diagonalCollides(entity_i, entity_j);
			}

		}
	}

	// update position of entities that follow player or enemies to remove jitter
	for (int i = 0; i < registry.followers.size(); i++) {
		Follower& follower = registry.followers.components[i];
		Entity entity = registry.followers.entities[i];
		Position& position = registry.positions.get(entity);
		Position& owner_position = registry.positions.get(follower.owner);
		position.position = owner_position.position;
		position.position.y += follower.y_offset;
		position.position.x += follower.x_offset;
	}

	// 2nd phase of position correction strictly after first phase
	for (int i = 0; i < registry.secondaryFollowers.size(); i++) {
		SecondaryFollower& follower = registry.secondaryFollowers.components[i];
		Entity entity = registry.secondaryFollowers.entities[i];
		Position& position = registry.positions.get(entity);
		Position& owner_position = registry.positions.get(follower.owner);
		position.position = owner_position.position;
		position.position.y += follower.y_offset;
		position.position.x += follower.x_offset;
	}

}
