
#include "cocos2d.h"
#include "Game.h"
#include "Player\Weapons\Weapon.h"
#include "Freezer.h"
#include "HUD\DebugGUI.h"

Freezer::Freezer(Level* level)
	:Weapon(Type::Freezer, level),
	_time(0),
	_isFiring(false)
{
	level = _level;
	_sprite = Sprite::create("Textures/freezer.png");
	_sprite->setPosition(Vec2(100, 100));
}

Freezer::~Freezer()
{

}

void Freezer::update(float dt)
{
	auto player = Game::getInstance()->getPlayer();

	_time += dt;

	if (_isFiring && _time > (1.0f / player->fireRate))
	{
		_time = 0;

		Bullet bullet;
		bullet.Damage = 50;
		bullet.Direction = player->ifMovingRight() ? Vec2(1, 0) : Vec2(-1, 0);
		bullet.DistanceLeft = 10000;
		bullet.ShotByPlayer = true;
		bullet.Speed = 1;
		bullet.Node = Sprite::create("Textures/bullet1.png");
		bullet.Node->setPosition(player->getPosition());
		bullet.Luj = true;

		_level->addBullet(bullet);
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
