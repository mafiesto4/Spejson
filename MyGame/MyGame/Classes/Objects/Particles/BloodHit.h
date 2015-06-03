
#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class BloodHit : public ParticleExplosion
{
	Color3B _color;
	float _scale, _alpha;

	BloodHit(Color3B color, float scale, float alpha);

public:

	static BloodHit* createWithTotalParticles(int numberOfParticles, Color3B color, float scale, float alpha);

protected:

	virtual bool initWithTotalParticles(int numberOfParticles);
};
