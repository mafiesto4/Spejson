#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Player\Weapons\Pistol\Pistol.h"
#include "Levels\Level.h"

using namespace cocos2d;

Pistol::Pistol(Level* level)
	:Weapon(Type::Pistol, level)
{
	_sprite = Sprite::create("Textures/pistol.png");
	_sprite->setPosition(Vec2(100, 100));
}

Pistol::~Pistol()
{
}

void Pistol::update(float dt)
{

}

void Pistol::onMouseDown(Vec2 pos)
{
	auto player = Game::getInstance()->getPlayer();

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

void Pistol::onMouseUp(Vec2 pos)
{
}
