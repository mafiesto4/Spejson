#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Player\Weapons\Pistol\Pistol.h"
#include "Levels\Level.h"

using namespace cocos2d;

Pistol::Pistol()
{
		_weaponImage = Sprite::create("Textures/pistol.png");
		_weaponImage->setPosition(Vec2(100, 100));
		_weaponImage->setTag(555);
}

Pistol::~Pistol()
{
	if (_weaponImage)
	{
		_weaponImage->removeFromPhysicsWorld();
		_weaponImage->removeAllChildren();
		_weaponImage->removeFromParentAndCleanup(true);
		_weaponImage = nullptr;
	}
}

void Pistol::update(float dt)
{

}



void Pistol::onMouseDown(Vec2 pos, Level* level)
{
	
	

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto player = Game::getInstance()->getPlayer();

	auto playerPos = player->getPosition();
	auto camPos = level->getCamera()->getPosition();

	Bullet bullet;
	bullet.Damage = 10;
	//bullet.Direction = location - (playerPos - camPos + visibleSize * 0.5);

	if (player->ifMovingRight())
		bullet.Direction = Vec2(1, 0);
	else
	{
		bullet.Direction = Vec2(-1, 0);
	}

	bullet.Position = playerPos;
	bullet.DistanceLeft = 10000;
	bullet.ShotByPlayer = true;
	bullet.Speed = 1;

	level->addBullet(bullet);
}

void Pistol::onMouseUp(Vec2 pos, Level* level)
{

}
