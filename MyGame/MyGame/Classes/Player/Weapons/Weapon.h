#pragma once

#include "cocos2d.h"

class Level;

class Weapon
{
public:

	enum class Type
	{
		Pistol = 0,
		MachineGun = 1,
		Freezer = 2,

		MAX = 3
	};

protected:

	Type _type;
	Level* _level;
	cocos2d::Sprite* _sprite;
	int _fireRate;
	bool isAutomatic;

public:

	Weapon(Type type, Level* level)
		:_type(type),
		_level(level),
		_sprite(nullptr)
	{
	}

	~Weapon()
	{
		if (_sprite)
		{
			_sprite->removeAllChildren();
			_sprite->removeFromParentAndCleanup(true);
			_sprite = nullptr;
		}
	}

	cocos2d::Sprite* getSprite() const
	{
		return _sprite;
	}

	Type getType() const
	{
		return _type;
	}

	virtual void update(float dt) = 0;

public:

	virtual void onMouseDown(cocos2d::Vec2 pos)
	{

	}

	virtual void onMouseUp(cocos2d::Vec2 pos)
	{
	}
};