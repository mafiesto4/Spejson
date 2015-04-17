#pragma once

#include <string> 
#include "cocos2d.h"
#include "Levels\Level1.h"

using namespace cocos2d;

class Opponent
{
protected:
	int _hp;
	std::string _name;
	Sprite* _image;
	PhysicsBody* _body;
	

	// Movement
	Vec2 navPoints[2];
	int navTargetPointIndex;


public:

	Opponent(std::string name);
	~Opponent();

	virtual void update(float dt) = 0;

protected:
	void opponentMovement();
	
};