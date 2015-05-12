#pragma once

#include <string> 
#include "cocos2d.h"
#include "Player\Weapons\Weapon.h"
#include "Levels\Level.h"

using namespace cocos2d;

class Pistol : public Weapon
{
public:
	Pistol();
	~Pistol();
	void update(float dt) override;

	void onMouseDown(Vec2 pos, Level* level) override;
	void onMouseUp(Vec2 pos, Level* level) override;
};