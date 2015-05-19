#pragma once

#include <string> 
#include "cocos2d.h"
#include "Entity.h"

using namespace cocos2d;

class Shop : public Entity
{
private:

	bool _isOver;
	Sprite* _image;

public:

	Shop(Chunk* parent, Vec2 pos);
	~Shop();

	bool update(class Level* level, float dt) override;

	cocos2d::Sprite* getNode() const
	{
		return _image;
	}
};