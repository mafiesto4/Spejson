#pragma once

#include <string> 
#include "cocos2d.h"
#include "Player\Weapons\Weapon.h"
#include "Levels\Level.h"

using namespace cocos2d;



class MachineGun : public Weapon
{
private:
	bool _firing=false;
	Level* level;
	float time = 0;


public:
	MachineGun(Level* _level);
	~MachineGun();
	void update(float dt) override;

	void onMouseDown(Vec2 pos, Level* level) override;
	void onMouseUp(Vec2 pos, Level* level) override;

	bool ifFiring()
	{
		return _firing;
	}

	bool switchFire()
	{
		if (_firing){ _firing = false; return false;}
		else{_firing = true; return true;}
	}
};