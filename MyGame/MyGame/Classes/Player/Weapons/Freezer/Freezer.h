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
	int _ammo;

protected:

	const char* getSpritePath() const override
	{
		return "Textures/freezer.png";
	}

public:

	Freezer(Level* _level);
	~Freezer();

	void update(float dt) override;

	void onMouseDown(Vec2 pos) override;
	void onMouseUp(Vec2 pos) override;

public:

	void addAmmo(int value) override
	{
		if (CanUse)
		_ammo += value;
	}

	int getAmmo() const override
	{
		return _ammo;
	}
};