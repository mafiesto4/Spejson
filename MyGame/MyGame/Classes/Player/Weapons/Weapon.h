#pragma once

#include <string> 
#include "cocos2d.h"
//#include "Levels\Level.h"

using namespace cocos2d;
class Level;


class Weapon {
protected:
	Sprite* _weaponImage;
	int _damage;
	int _fireRate;
	bool isAutomatic;
	int _maxAmmo;
	int _ammo = 0;


public:
	//Weapon();
	//virtual ~Weapon();


	virtual void update(float dt) = 0;

	void weaponSetup();

	void setupForNode(Node* level);
	Node* getSprite();
	virtual void onMouseDown(Vec2 pos, Level* level)
	{


	}
	virtual void onMouseUp(Vec2 pos, Level* level)
	{

	}

	void addAmmo(int ammo)
	{
	_ammo += ammo;
	if (_ammo > _maxAmmo)
	{
	_ammo = _maxAmmo;
	}
	}

	void decreaseAmmo(int ammo)
	{
	_ammo -= ammo;
	if (_ammo <= 0)
	{
	_ammo = 0;
	}
	}

	int getAmmo(){ return _ammo; }


};