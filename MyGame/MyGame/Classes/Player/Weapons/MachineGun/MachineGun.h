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

public:

	MachineGun(Level* level);
	~MachineGun();

	void update(float dt) override;

	void onMouseDown(Vec2 pos) override;
	void onMouseUp(Vec2 pos) override;

	bool IsFiring() const
	{
		return _isFiring;
	}
};
