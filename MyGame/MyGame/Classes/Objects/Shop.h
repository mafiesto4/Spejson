#pragma once

#include "cocos2d.h"
#include "Entity.h"

using namespace cocos2d;

#define SHOP_ANIMATION_TIME 1.0f

extern bool AnyShopInUse;

class Shop : public Entity
{
private:

	bool _isOver;
	float _animTime;
	
	Sprite* _image;
	Sprite* _logo;
	Sprite* _info;

	Sprite* _itemAmmo;
	Sprite* _itemMed;
	Sprite* _itemFreezer;

public:

	Shop(Chunk* parent, Vec2 pos);
	~Shop();

	bool update(class Level* level, float dt) override;
	void onMouseDown(EventMouse* event) override;
	void onMouseUp(EventMouse* event) override;

	Sprite* getNode() const
	{
		return _image;
	}
};