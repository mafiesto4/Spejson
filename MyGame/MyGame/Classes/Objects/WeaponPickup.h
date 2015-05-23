#pragma once

#include "cocos2d.h"
#include "Entity.h"
#include "../Player/Weapons/Weapon.h"

using namespace cocos2d;

class WeaponPickup : public Entity
{
private:

	Sprite* _image;
	Weapon::Type _type;

public:

	WeaponPickup(Chunk* parent, Vec2 pos, Weapon::Type type);
	~WeaponPickup();

	bool update(class Level* level, float dt) override;

	cocos2d::Sprite* getNode() const
	{
		return _image;
	}
};
