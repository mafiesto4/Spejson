#include <string>
#include "cocos2d.h"
#include "Opponent\Opponent.h"
#include "Shooting.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "../../HUD/DebugGUI.h"
#include "../../Levels/Chunk.h"

using namespace std;
using namespace cocos2d;

Shooting::Shooting(Chunk* parent, Vec2 p1, Vec2 p2)
	:Opponent(parent),
	_p1(p1),
	_p2(p2),
	_timeAcc(0)
{
	_node = Sprite::create("Textures/facebook.png");
	_node->setPosition(p1);
	parent->addChild(_node);
}

Shooting::~Shooting()
{
	if (_node)
	{
		_node->removeFromPhysicsWorld();
		_node->removeAllChildren();
		_node->removeFromParentAndCleanup(true);
		_node = nullptr;
	}
}

bool Shooting::update(Level* level, float dt)
{
	// Base
	if (Opponent::update(level, dt))
	{
		return true;
	}

	// Check if can 'see' a player
	auto player = Game::getInstance()->getPlayer();
	_state = State::Shooting;


	// Switch state
	switch (_state)
	{
		case State::Undefined:
		{
			auto anim = MoveTo::create(calMoveDuration(), _p1);
			anim->setTag(0);
			_node->runAction(anim);
			_state = State::PatrollingA;
		}
		break;

		case State::PatrollingA:
		{
			if (!_node->getActionByTag(0))
			{
				auto anim = MoveTo::create(calMoveDuration(), _p2);
				anim->setTag(0);
				_node->runAction(anim);
				_state = State::PatrollingB;

			}
		}
		break;

		case State::PatrollingB:
		{
			if (!_node->getActionByTag(0))
			{
				auto anim = MoveTo::create(calMoveDuration(), _p1);
				anim->setTag(0);
				_node->runAction(anim);
				_state = State::PatrollingA;
			}
		}
		break;

		case State::Shooting:
		{
			_timeAcc += dt;

			if (_timeAcc > 0.5f)
			{
				_timeAcc = 0;

				Bullet bullet;
				bullet.Damage = 1;
				auto pos = _node->getPosition() + _parent->getPosition();
				bullet.Direction = player->getPosition() - pos;
				bullet.DistanceLeft = 10000;
				bullet.ShotByPlayer = false;
				bullet.Speed = 1;
				bullet.Node = Sprite::create("Textures/bullet1.png");
				bullet.Node->setPosition(pos);

				level->addBullet(bullet);
			}
		}
		break;


		case State::SearchForPlayer:
		{

		}
		break;
	}

	return false;
}
