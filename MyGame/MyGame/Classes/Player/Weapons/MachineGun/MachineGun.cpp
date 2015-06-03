#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Player\Weapons\Weapon.h"
#include "MachineGun.h"
#include "Levels\Level.h"
#include "HUD\DebugGUI.h"
#include <SimpleAudioEngine.h>

MachineGun::MachineGun(Level* level)
	:Weapon(Type::MachineGun, level),
	_time(1000),
	_isFiring(false),
	_reloadTimer(0)
{
	// Setup ammo
	_MgAmmo = 20;
	_magazine = 20;
	_MgMaxAmmo = 20;
}

MachineGun::~MachineGun()
{
}

void MachineGun::update(float dt)
{
	auto player = Game::getInstance()->getPlayer();

	_time += dt;

	if (_isFiring && (_time > 0.1f / player->fireRate) && _magazine > 0)
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

		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/shot_Minigun.wav");

		_level->addBullet(bullet);
		_magazine--;
	}
	else if (_magazine <= 0)
	{
		if (_reloadTimer <= 0 && _MgAmmo > 0)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/reload.wav");
		}
		reload(dt);
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

void MachineGun::reload(float dt)
{
	_reloadTimer += dt;

	if (_reloadTimer > 1)
	{
		_reloadTimer = 0;
		if (_MgAmmo >= _MgMaxAmmo)
		{
			_magazine += _MgMaxAmmo;
			_MgAmmo -= _MgMaxAmmo;
		}
		else
		{
			_magazine += _MgAmmo;
			_MgAmmo = 0;
		}
	}
}
