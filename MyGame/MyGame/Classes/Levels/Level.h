
#pragma once

#include <vector>
#include <cocos2d.h>
#include <string.h>
#include "HUD\GameHUD.h"
#include "physics\CCPhysicsWorld.h"
#include "Bullet.h"
#include "Levels/Lava.h"
#include "Objects/Coin.h"
#include "Player\Weapons\Weapon.h"
#include "Player\Weapons\MachineGun\MachineGun.h"
#include "Player\Weapons\Pistol\Pistol.h"

// Forward declarations
class Chunk;
class Opponent;

using namespace cocos2d;
using namespace std;

class Level : public Layer
{
	friend Level;

private:

	vector<Bullet> _bullets;
	vector<Opponent*> _opponents;
	vector<Chunk*> _chunks;
	Chunk* _rootChunk;
	Chunk* _closestPToPlayerChunk;
	PhysicsWorld* m_world;
	GameHUD* _hud;
	MachineGun* mGun;
	Pistol* pistol;
	Lava* _lava;
	bool onContactBegin(EventCustom* event, const PhysicsContact& contact);

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

	// a selector callback
	void menuCloseCallback(Object* pSender);

	CREATE_FUNC(Level);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

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
