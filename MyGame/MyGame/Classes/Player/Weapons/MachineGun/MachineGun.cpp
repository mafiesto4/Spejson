#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Player\Weapons\Weapon.h"
#include "Player\Weapons\MachineGun\MachineGun.h"
#include "Levels\Level.h"
#include "HUD\DebugGUI.h"

MachineGun::MachineGun(Level* _level)
{
			level = _level; 
			_weaponImage = Sprite::create("Textures/mGun.jpg");
			_weaponImage->setPosition(Vec2(100, 100));
			_weaponImage->setTag(555);
			isAutomatic = true;
			_maxAmmo = 30;
}

MachineGun::~MachineGun()
{
	if (_weaponImage)
	{
		_weaponImage->removeFromPhysicsWorld();
		_weaponImage->removeAllChildren();
		_weaponImage->removeFromParentAndCleanup(true);
		CC_SAFE_RELEASE_NULL(_weaponImage);
	}
}

void MachineGun::update(float dt)
{
	auto player = Game::getInstance()->getPlayer();

	DebugGUI::setVal(6, "dt", dt);
	time += dt;


	if (ifFiring() && time>0.1f/(player->fireRate) && _ammo > 0)
	{
		time = 0;
		
		Size visibleSize = Director::getInstance()->getVisibleSize();

		

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

		bullet.DistanceLeft = 10000;
		bullet.ShotByPlayer = true;
		bullet.Speed = 1;
		bullet.Node = Sprite::create("Textures/bullet1.png");
		bullet.Node->setPosition(playerPos);

		level->addBullet(bullet);
		decreaseAmmo(1);
	}
}



void MachineGun::onMouseDown(Vec2 pos, Level* level)
{
	switchFire();
}


void MachineGun::onMouseUp(Vec2 pos, Level* level)
{
	switchFire();
}