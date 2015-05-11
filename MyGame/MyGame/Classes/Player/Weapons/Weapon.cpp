#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Player\Weapons\Weapon.h"
#include "Levels\Level.h"

using namespace cocos2d;

void Weapon::setupForNode(Node* level)
{

	level->addChild(_weaponImage);
}

Node* Weapon::getSprite()
{
	return _weaponImage;
}

/*Weapon::Weapon()
{

}

Weapon::~Weapon()
{
	if (_weaponImage)
	{
		_weaponImage->removeFromPhysicsWorld();
		_weaponImage->removeAllChildren();
		_weaponImage->removeFromParentAndCleanup(true);
		_weaponImage = nullptr;
	}
}*/



