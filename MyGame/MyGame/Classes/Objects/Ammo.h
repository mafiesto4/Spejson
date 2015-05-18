#pragma once

#include <string> 
#include "cocos2d.h"
using namespace cocos2d;

class Ammo
{
private:
	int _value;
	Sprite* _image;

public:
	Ammo(Node& level, Vec2 _pos);
	~Ammo();
	void setupForLevel();
	Node* getSprite();

	int getValue()
	{
		return _value;
	}
};