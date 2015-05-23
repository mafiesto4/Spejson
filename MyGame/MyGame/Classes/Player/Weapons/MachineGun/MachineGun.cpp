#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Player\Weapons\Weapon.h"
#include "MachineGun.h"
#include "Levels\Level.h"
#include "HUD\DebugGUI.h"

MachineGun::MachineGun(Level* level)
	:Weapon(Type::MachineGun, level),
	_time(0),
	_isFiring(false)
{
	_sprite = Sprite::create("Textures/mGun.jpg");
	_sprite->setPosition(Vec2(100, 100));
}

MachineGun::~MachineGun()
{
}

void MachineGun::update(float dt)
{
	auto player = Game::getInstance()->getPlayer();

	_time += dt;

	if (_isFiring && (_time > 0.1f / player->fireRate))
	{
		_time = 0;

		Bullet bullet;
		bullet.Damage = 10;
		bullet.Direction = player->ifMovingRight() ? Vec2(1, 0) : Vec2(-1, 0);
		bullet.DistanceLeft = 10000;
		bullet.ShotByPlayer = true;
		bullet.Speed = 1;
		bullet.Node = Sprite::create("Textures/bullet1.png");
		bullet.Node->setPosition(player->getPosition());
		bullet.Luj = false;

		_level->addBullet(bullet);
	}
}

void MachineGun::onMouseDown(Vec2 pos)
{
	_isFiring = true;
}

void MachineGun::onMouseUp(Vec2 pos)
{
	_isFiring = false;
}
