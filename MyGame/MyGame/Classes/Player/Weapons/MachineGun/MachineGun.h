#pragma once

#include "cocos2d.h"
#include "Player\Weapons\Weapon.h"
#include "Levels\Level.h"

using namespace cocos2d;

class MachineGun : public Weapon
{
private:

	bool _isFiring;
	float _time;
	float _reloadTimer;
	int _MgAmmo;
	int _MgMaxAmmo;

protected:

	const char* getSpritePath() const override
	{
		return "Textures/mGun.jpg";
	}

public:

	MachineGun(Level* level);
	~MachineGun();

	void update(float dt) override;

	void onMouseDown(Vec2 pos) override;
	void onMouseUp(Vec2 pos) override;

public:

	bool IsFiring() const
	{
		return _isFiring;
	}

	void addAmmo(int value) override
	{
		_MgAmmo += value;
	}

	int getAmmo() const override
	{
		return _MgAmmo;
	}

	void reload(float dt);
};
