#pragma once

#include "cocos2d.h"
#include "Opponent/Opponent.h"

using namespace cocos2d;

extern int Bosses;

class Boss : public Opponent
{
private:

	float _sleep;
	Vec2 _p1, _p2;
	Vec2 _target;

	void startAnim();

public:

	Boss(Chunk* parent, Vec2 p1, Vec2 p2);
	~Boss();

	bool update(Level* level, float dt) override;
};
