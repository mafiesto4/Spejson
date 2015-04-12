
#pragma once

#include <cocos2d.h>
#include <string.h>
#include "HUD\GameHUD.h"
#include "physics\CCPhysicsWorld.h"

using namespace cocos2d;

class Level1 : public LayerColor
{
private:

	PhysicsWorld* m_world;
	bool onContactBegin(EventCustom* event, const PhysicsContact& contact);

public:

	GameHUD* _hud;

	~Level1();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static Scene* createScene();

	virtual bool init();
	virtual void update(float dt);

	// a selector callback
	void menuCloseCallback(Object* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Level1);

	void setPhyWorld(PhysicsWorld* world){ m_world = world; }

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	void addNewSpriteAtPosition(Point p);

	void addBrick1(Point p);
};
