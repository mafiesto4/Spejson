#pragma once

#include <string> 
#include "cocos2d.h"
#include "Entity.h"

using namespace cocos2d;

class Ammo : public Entity
{
private:

	int _value;
	Sprite* _image;

public:

	Ammo(Chunk* parent, Vec2 pos);
	~Ammo();

	bool update(class Level* level, float dt) override;

	cocos2d::Sprite* getNode() const
	{
		return _image;
	}

	int getValue()
	{
		return _value;
	}
};