
#pragma once

#include "cocos2d.h"

class GameHUD : public cocos2d::Layer
{
private:

	cocos2d::Label* _score;
	cocos2d::Label* _hp;
	cocos2d::Label* _ammo;
	cocos2d::Label* _lifes;

public:

	virtual bool init();
	virtual void update(float dt);

	CREATE_FUNC(GameHUD);
};
