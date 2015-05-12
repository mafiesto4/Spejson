
#pragma once

#include <cocos2d.h>

class Lava : public cocos2d::LayerGradient
{
public:

	float getLevel();

	virtual bool init();
	virtual void update(float dt);
	CREATE_FUNC(Lava);
};
