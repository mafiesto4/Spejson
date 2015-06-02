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
	_timeAcc(0),
	_p1(p1 + Vec2(0, 64)),
	_p2(p2 + Vec2(0, 64))
{
	_node = Sprite::create("Textures/facebook.png");
	_node->setPosition(p1);
	parent->addChild(_node);
}

Shooting::~Shooting()
{
}

bool Shooting::update(Level* level, float dt)
{
	// Base
	if (Opponent::update(level, dt))
		return true;
	if (Opponent::postUpdate(dt))
	{
		auto anim = _node->getActionByTag(0);
		if (anim)
		{
			_state = State::Undefined;
			_node->stopActionByTag(0);
		}
		return false;
	}

	// Cache data
	auto player = Game::getInstance()->getPlayer();
	Vec2 playerPos = player->getPosition();
	Vec2 parentPos = _parent->getPosition();
	Vec2 pos = _node->getPosition();
	Vec2 posWS = pos + parentPos;

	// Check if can 'see' a player
	const int opponentOffsetYRange = 200;
	const int opponentViewRange = 700;
	bool seePlayer = posWS.y < playerPos.y + opponentOffsetYRange && posWS.y > playerPos.y - opponentOffsetYRange && posWS.distance(playerPos) < opponentViewRange;
	if (_state == State::Shooting && !seePlayer)
	{
		_state = State::PatrollingA;
	}
	else if (seePlayer)
	{
		_state = State::Shooting;
	}

	// Switch state
	switch (_state)
	{
		case State::Undefined:
		{
			_node->setPosition(_p1);
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
				_node->setScaleX(-1);
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
				_node->setScaleX(1);
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
				bullet.Direction = playerPos - posWS;
				bullet.DistanceLeft = 10000;
				bullet.ShotByPlayer = false;
				bullet.Speed = 1;
				bullet.Node = Sprite::create("Textures/bullet1.png");
				bullet.Node->setPosition(posWS);

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
