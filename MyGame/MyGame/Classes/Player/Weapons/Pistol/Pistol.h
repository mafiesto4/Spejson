#pragma once

#include "cocos2d.h"
#include "Player\Weapons\Weapon.h"
#include "Levels\Level.h"

using namespace cocos2d;

class Pistol : public Weapon
{
protected:

	const char* getSpritePath() const override
	{
		return "Textures/pistol.png";
	}

public:

	Pistol(Level* level);
	~Pistol();

	void update(float dt) override;

	void onMouseDown(Vec2 pos) override;
	void onMouseUp(Vec2 pos) override;
};
