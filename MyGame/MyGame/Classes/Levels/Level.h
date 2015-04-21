
#pragma once

#include <vector>
#include <cocos2d.h>
#include <string.h>
#include "HUD\GameHUD.h"
#include "physics\CCPhysicsWorld.h"
#include "Bullet.h"
#include "Levels\Chunk.h"

class Opponent;

using namespace cocos2d;
using namespace std;

class Level : public LayerColor
{
	friend Level;

private:

	vector<Bullet> _bullets;
	vector<Opponent*> _opponents;
	vector<Chunk*> _chunks;
	PhysicsWorld* m_world;
	GameHUD* _hud;
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

	void addBrick1(Point p);
	void shoot(Bullet& bullet);
};
