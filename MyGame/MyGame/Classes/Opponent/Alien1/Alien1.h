#pragma once

#include "cocos2d.h"
#include "Opponent/Opponent.h"

using namespace cocos2d;

class Alien1 : public Opponent
{
private:

	Vec2 _p1, _p2;

	void startAnim();

public:

	Alien1(Chunk* parent, Vec2 p1, Vec2 p2);
	~Alien1();

	bool update(Level* level, float dt) override;

public:

	float calMoveDuration()
	{
		//return 4;
		return _p1.distance(_p2) / getSpeed();
	}
};