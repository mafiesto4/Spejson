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

private:

	cocos2d::Sprite* _sprite;

protected:

	Type _type;
	Level* _level;
	int _fireRate;
	bool isAutomatic;
	int _magazine;

	virtual const char* getSpritePath() const = 0;

public:

	// True if player can use that wepon, otherwise false
	bool CanUse;

	Weapon(Type type, Level* level)
		:_type(type),
		_level(level),
		_sprite(nullptr),
		_magazine(0),
		CanUse(false)
	{
	}

	~Weapon()
	{
		onDeselect();
	}

	Type getType() const
	{
		return _type;
	}

	virtual void onSelect(cocos2d::Node* parent)
	{
		_sprite = cocos2d::Sprite::create(getSpritePath());
		_sprite->setPosition(cocos2d::Vec2(100, 100));
		parent->addChild(_sprite);
	}

	virtual void update(float dt) = 0;

	virtual void onDeselect()
	{
		if (_sprite)
		{
			_sprite->removeAllChildren();
			_sprite->removeFromParentAndCleanup(true);
			_sprite = nullptr;
		}
	}

public:

	virtual void addAmmo(int value)
	{
	}

	virtual int getAmmo() const
	{
		return 0;
	}

	int getMagazine() const
	{
		return _magazine; 
	}

public:

	virtual void onMouseDown(cocos2d::Vec2 pos)
	{

	}

	virtual void onMouseUp(cocos2d::Vec2 pos)
	{
	}
};