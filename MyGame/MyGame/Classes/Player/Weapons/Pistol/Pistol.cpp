#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Player\Weapons\Pistol\Pistol.h"

void Pistol::setupForPlayer(Node* level)
{
	
	if (_weaponImage)
	{
		_weaponImage->removeFromParent();
	}
	else
	{
		_weaponImage = Sprite::create("Textures/pistol.png");
		_weaponImage->setPosition(Vec2(100, 100));
	}


	level->addChild(_weaponImage);
}