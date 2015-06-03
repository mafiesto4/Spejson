
#include "cocos2d.h"
#include "Game.h"
#include "Player\Weapons\Weapon.h"
#include "Freezer.h"
#include "HUD\DebugGUI.h"
#include <SimpleAudioEngine.h>

Freezer::Freezer(Level* level)
	:Weapon(Type::Freezer, level),
	_time(1000),
	_isFiring(false),
	_ammo(3)
{
	_magazine = 1;
}

Freezer::~Freezer()
{

}

void Freezer::update(float dt)
{
	auto player = Game::getInstance()->getPlayer();

	_time += dt;

	if (_ammo > 0 && _time > (1.0f / player->fireRate))
	{
		if (_isFiring)
		{
			_time = 0;

			Bullet bullet;
			bullet.Damage = 50;
			bullet.Direction = player->ifMovingRight() ? Vec2(1, 0) : Vec2(-1, 0);
			bullet.DistanceLeft = 10000;
			bullet.ShotByPlayer = true;
			bullet.Speed = 1;
			bullet.Node = Sprite::create("Textures/rocket.png");
			bullet.Node->setPosition(player->getPosition());
			bullet.Luj = true;

			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/shot_Freezer.wav");

			_level->addBullet(bullet);
			_magazine = 0;
			_ammo--;
		}
		else
		{
			if (_ammo > 0)
			{
				_magazine = 1;
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/reload.wav");
			}
		}
	}
	else
	{
		_magazine = 0;
	}
}

void Freezer::onMouseDown(Vec2 pos)
{
	_isFiring = true;
}

void Freezer::onMouseUp(Vec2 pos)
{
	_isFiring = false;
}
