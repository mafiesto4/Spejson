
#pragma once

#include <vector>
#include <cocos2d.h>
#include <string.h>
#include "HUD\GameHUD.h"
#include "physics\CCPhysicsWorld.h"
#include "Bullet.h"
#include "Levels/Lava.h"
#include "Objects/Coin.h"

// Forward declarations
class Chunk;
class Opponent;

using namespace cocos2d;
using namespace std;

class Level : public LayerColor
{
	friend Level;

private:

	vector<Bullet> _bullets;
	vector<Opponent*> _opponents;
	vector <Coin*> _coins;
	vector<Chunk*> _chunks;
	Chunk* _rootChunk;
	Chunk* _closestPToPlayerChunk;
	PhysicsWorld* m_world;
	GameHUD* _hud;
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

	virtual bool init();
	virtual void update(float dt);
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

	// a selector callback
	void menuCloseCallback(Object* pSender);

	CREATE_FUNC(Level);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	void addNewSpriteAtPosition(Point p);

	// add bullet to the list
	void addBuulet(Bullet& bullet);

	////////////////////////////////////////////////////
	// Chunks stuff

	// cleans all chunks data (use with caution)
	void cleanAllChunks();

	// creates initial world state (generates all chunks and moves player to the start)
	void setupInitialMap();

	// updates all chnuks state (removes old ones and generates new ones)
	void flushChunks();

	// add single chunk to the game
	void addChunk(Chunk* chunk);

	////////////////////////////////////////////////////
};
