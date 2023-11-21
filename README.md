# Aria: Whispers of Darkness
In the eerie village of Grimhaven, shrouded in perpetual mist and shadows, Aria's journey begins. 

# Milestone 3
Below is the documentation for our `README.md` submission for Milestone 3.

## Entry Points
- **Sprite sheet animation** (prior milestone fix): adapted to accomodate sprite sheets with rows and partitions frames into animation states. 
[Initializing sprite sheets](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/8c1ac9ec1c7e2da38d80536bf5c18bd55f0c68fd/src/render_system_init.cpp#L296), 
[Passing in shader parameters](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/8c1ac9ec1c7e2da38d80536bf5c18bd55f0c68fd/src/render_system.cpp#L91), 
[Shader implementation](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/8c1ac9ec1c7e2da38d80536bf5c18bd55f0c68fd/shaders/animated.fs.glsl#L84).
- **Simple rendering effects** (prior milestone fix): 2 shader effects, 
[Spotlight](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/8c1ac9ec1c7e2da38d80536bf5c18bd55f0c68fd/shaders/screen_darken.fs.glsl#L21) and 
[Rainbow](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/8c1ac9ec1c7e2da38d80536bf5c18bd55f0c68fd/shaders/animated.fs.glsl#L70).
- [**Consistent game resolution**](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/5f074bd1f09434511ea25e1ac449e401fb17adc4/src/world_system.cpp#L96)
- **Basic Integrated Assets**:
Enemy Sprites ([water](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/M3_Submission/data/textures/water_enemy.png), [fire](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/8c1ac9ec1c7e2da38d80536bf5c18bd55f0c68fd/data/textures/fire_enemy.png), [earth](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/M3_Submission/data/textures/earth_enemy.png), [lightning](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/M3_Submission/data/textures/lightning_enemy.png), [ghost](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/M3_Submission/data/textures/ghost.png)),
[Boss Sprites](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/M3_Submission/data/textures/fire_boss.png),
[Power-up Block Sprite](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/8c1ac9ec1c7e2da38d80536bf5c18bd55f0c68fd/data/textures/power_up_block.png),
[Projectile Select Display Sprite](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/8c1ac9ec1c7e2da38d80536bf5c18bd55f0c68fd/data/textures/projectile-select-display-purple.png),
[Portal Sprite](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/8c1ac9ec1c7e2da38d80536bf5c18bd55f0c68fd/data/textures/portal.png).
- **Advanced Integrated Assets**:
[New Music](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/M3_Submission/data/audio/boss_battle.wav), [Player Sprite](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/M3_Submission/data/textures/aria.png)
- **2D Dynamic Shadows**:
  - [Shadow entity](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/8c1ac9ec1c7e2da38d80536bf5c18bd55f0c68fd/src/components.hpp#L63)
  - [Shadow creation](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/8c1ac9ec1c7e2da38d80536bf5c18bd55f0c68fd/src/world_init.cpp#L245)
  - [Shadow vertex shader](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/main/shaders/shadow.vs.glsl), [Shadow fragment shader](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/main/shaders/shadow.fs.glsl)
  - [Dynamic shadow update](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/8c1ac9ec1c7e2da38d80536bf5c18bd55f0c68fd/src/physics_system.cpp#L131)
  - [Dim lights to simulate lighting and shadows](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/8c1ac9ec1c7e2da38d80536bf5c18bd55f0c68fd/shaders/screen_darken.fs.glsl#L43)
- **Enemy group behaviour / Cooperative planning**
  - [Enemies Flank Player](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/e0e34f09731f7c53edfcdf05e690c7d275f4dec4/src/ai_system.cpp#L81)
  - [Enemies Heal Allies](https://github.students.cs.ubc.ca/CPSC427-2023W-T1/Team06Aria/blob/e0e34f09731f7c53edfcdf05e690c7d275f4dec4/src/ai_system.cpp#L76)

## Development Plan
Most of the work in this milestone aligned with our development plan for this milestone.

### Aligned with the proposal:
- Gameplay extended to ~6-7 minutes.
- Mini-bosses implemented.
- Finished player sprite.

### Discreprancies:
- Did not finish persistency (save/load).
- Moving complex motion implementation to next milestone.
- Did not add knockback to enemy collisions.

## ECS Design Pattern
The ECS design pattern has not been changed since Milestone 1 and 2.

## Game Entities
- Player
- Enemy
- Terrain
- Exit Door
- Projectile
- Text
- Health Bar
- Floor
- PowerUpBlock

### New Game Entities:
- Shadow
- Obstacle

## Actionable Components
- Velocity
- Position
- Collision
- Direction
- Invulnerable Timer
- Death Timer
- Resources
- Mesh
- Render Request
- Screen State
- Color
- Projectile
- Text
- CharacterProjectileType
- Collidable
- Win Timer
- Animation
- Power Up

### New Actionable Components
- Follower

## Entity Component Diagram
Highlight any changes versus the previous milestone.
![ECS diagram](docu/images/M3_ECS_diagram.png)
