#pragma once

#include <cocos2d.h>
#include "Entity.h"

class Ladder : public Entity
{
	friend class Chunk;

private:

	cocos2d::Sprite* _sprite;
	bool ifActive=false;
	Ladder(const cocos2d::Vec2& location, int height);

public:

	void update(class Chunk* parent, float dt) override;

	cocos2d::Sprite* getNode() const override
	{
		return _sprite;
	}
};
