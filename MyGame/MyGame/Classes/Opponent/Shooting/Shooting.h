#pragma once

#include <string> 
#include "cocos2d.h"
#include "Opponent/Opponent.h"

using namespace cocos2d;

class Shooting : public Opponent
{
private:

	float _timeAcc;
	Vec2 _p1, _p2;

	void startAnim();

public:

	Shooting(Chunk* parent, Vec2 p1, Vec2 p2);
	~Shooting();

	bool update(Level* level, float dt) override;

public:

	Color3B getBloodColor() const override
	{
		return Color3B(20, 116, 50);
	}

	float calMoveDuration()
	{
		return _p1.distance(_p2) / getSpeed();
	}
};