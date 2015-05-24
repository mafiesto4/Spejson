#include <string>
#include "cocos2d.h"
#include "Opponent\Opponent.h"
#include "Fly.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "../../HUD/DebugGUI.h"
#include "../../Levels/Chunk.h"

using namespace std;
using namespace cocos2d;

Fly::Fly(Chunk* parent, Vec2 p1, Vec2 p2)
	:Opponent(parent),
	_p1(p1),
	_p2(p2),
	_timeAcc(0),
	_speed(300),
	_randDir(1,0)
{
	_node = Sprite::create("Textures/youtube.png");
	_node->setPosition(p1);
	parent->addChild(_node, 10000);
}

Fly::~Fly()
{
}

bool Fly::update(Level* level, float dt)
{
	// Base
	if (Opponent::update(level, dt))
		return true;
	if (Opponent::postUpdate(dt))
		return false;

	// Cache data
	auto player = Game::getInstance()->getPlayer();
	Vec2 playerPos = player->getPosition();
	Vec2 parentPos = _parent->getPosition();
	Vec2 pos = _node->getPosition();
	Vec2 posWS = pos + parentPos;

	// Check if can 'see' a player
	const int opponentViewRange = 700;
	bool seePlayer = posWS.distance(playerPos) < opponentViewRange;
	_state = seePlayer ? State::FollowingPlayer : State::SearchForPlayer;

	// Randomize speed factor
	_speed = (rand() % 200) + 50;

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

		case State::SearchForPlayer:
		case State::FollowingPlayer:
		{
			// Cache some data
			Vec2 dir = playerPos - posWS;
			Vec2 dirNorm = dir;
			dirNorm.normalize();
			float dirLen = dir.length();

			// Sprawdz czy przypadkiem nie zblizylismy sie zbytnio do gracza
			const int minDistanceToPlayer = 200;
			Vec2 posDelta = Vec2::ZERO;
			if (dirLen > minDistanceToPlayer)
			{
				// Sprawdz czy nie oddalilismy sie przypadkiem od gracza
				if (dirLen < CHUNKS_DEFAULT_HEIGHT * 3)
				{
					// Poruszanie sie w kierunku gracza ze stala predkoscia i ale zmienym natezeniem dzwieku w osrodku Lorenza
					posDelta = dirNorm * _speed * dt;
				}
			}

			// Move like a jagger
			float coeff = dt / 40.0f;
			_randDir += Vec2((float)(rand() % 10) * coeff, (float)(rand() % 10) * coeff);
			_randDir.normalize();
			posDelta += _randDir * 1.2f;

			// Move if need to
			if (posDelta != Vec2::ZERO)
				_node->setPosition(pos + posDelta);

			// Shoot if can see a player
			if (seePlayer)
			{
				_timeAcc += dt;
				if (_timeAcc > 0.6f)
				{
					_timeAcc = 0;

					Bullet bullet;
					bullet.Damage = 1;
					bullet.Direction = dirNorm;
					bullet.DistanceLeft = 10000;
					bullet.ShotByPlayer = false;
					bullet.Speed = 1.5f;
					bullet.Node = Sprite::create("Textures/bullet1.png");
					bullet.Node->setPosition(posWS);

					level->addBullet(bullet);
				}
			}
		}
		break;
	}

	return false;
}
