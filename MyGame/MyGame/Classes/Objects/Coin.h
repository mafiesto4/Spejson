#pragma once

#include <string> 
#include "cocos2d.h"
#include "Entity.h"

using namespace cocos2d;

class Coin : public Entity
{
private:

	Sprite* _image;

public:

	Coin(Chunk* parent, Vec2 pos);
	~Coin();

	bool update(class Level* level, float dt) override;

	cocos2d::Sprite* getNode() const
	{
		return _image;
	}
};