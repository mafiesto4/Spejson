#pragma once

#include "cocos2d.h"
#include "Opponent/Opponent.h"

using namespace cocos2d;

class Fly : public Opponent
{
private:

	float _timeAcc, _speed;
	Vec2 _p1, _p2;
	Vec2 _randDir;

	void startAnim();

public:

	Fly(Chunk* parent, Vec2 p1, Vec2 p2);
	~Fly();

	bool update(Level* level, float dt) override;

public:

	Color3B getBloodColor() const override
	{
		return Color3B(1, 240, 70);
	}

	float calMoveDuration()
	{
		return _p1.distance(_p2) / getSpeed();
	}
	
	float getSpeed() const override
	{
		return _speed;
	}
};