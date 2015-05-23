#pragma once

#include <string> 
#include "cocos2d.h"
#include "Player\Weapons\Weapon.h"
#include "Levels\Level.h"

using namespace cocos2d;

class Freezer : public Weapon
{
private:

	bool _isFiring;
	float _time;

public:

	Freezer(Level* _level);
	~Freezer();

	void update(float dt) override;

	void onMouseDown(Vec2 pos) override;
	void onMouseUp(Vec2 pos) override;
};