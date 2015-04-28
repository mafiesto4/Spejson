#pragma once

#include <string> 
#include "cocos2d.h"
using namespace cocos2d;

class Coin 
{
private:
	int _value;
	Sprite* _image;

public:
	Coin(Node& level, Vec2 _pos);
	~Coin();
	void setupForLevel();
	Node* getSprite();
	
	int getValue(){
		return _value;
	}
};