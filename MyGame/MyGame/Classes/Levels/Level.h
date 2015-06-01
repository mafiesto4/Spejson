
#pragma once

#include <vector>
#include <cocos2d.h>
#include "HUD\GameHUD.h"
#include "physics\CCPhysicsWorld.h"
#include "Bullet.h"
#include "Levels/Lava.h"

// Forward declarations
class Chunk;
class Opponent;
class Background;

using namespace cocos2d;
using namespace std;

#define USE_PHYSICS_DEBUG_LAYER 0

class Level : public Layer
{
	friend Level;
	friend class Opponent;

private:

	vector<Bullet> _bullets;
	vector<Chunk*> _chunks;
	Chunk* _rootChunk;
	Chunk* _closestPToPlayerChunk;
	PhysicsWorld* m_world;
	GameHUD* _hud;
	Lava* _lava;
	Background* _background;

	// Camera movement
	#define CAM_MAX_VELOCITY 1000.0f
	#define CAM_VELOCITY_DUMP 0.0001f
	#define CAM_ACCEL 1.0f
	Camera* _camera;
	Vec2 _camVelocity;

public:

	~Level();

	static Scene* createScene();

	bool init() override;
	void update(float dt) override;

	CREATE_FUNC(Level);

	void onPlayerDeath();

	// add bullet to the list
	void addBullet(Bullet& bullet);

	Camera* getCamera()const { return _camera; }

	////////////////////////////////////////////////////
	// Chunks stuff

	// cleans all chunks data (use with caution)
	void cleanAllChunks();

	// creates initial world state (generates all chunks and moves player to the start)
	void setupInitialMap();

	// updates all chnuks state (removes old ones and generates new ones)
	void updateChunks(float dt);

	// add single chunk to the game
	void addChunk(Chunk* chunk);

	// Get chunk at point (may by null if nothing found)
	Chunk* chunkAtPoint(const Vec2& point);

	////////////////////////////////////////////////////
};
