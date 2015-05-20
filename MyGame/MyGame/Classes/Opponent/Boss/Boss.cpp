#include <string>
#include "cocos2d.h"
#include "Opponent\Opponent.h"
#include "Boss.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "../../HUD/DebugGUI.h"
#include "../../Levels/Chunk.h"
#include "../../Levels/ChunkTypes/BossFight.h"

using namespace std;
using namespace cocos2d;

Boss::Boss(Chunk* parent, Vec2 p1, Vec2 p2)
	:Opponent(parent),
	_p1(p1),
	_p2(p2),
	_timeAcc(0)
{
	_node = Sprite::create("Textures/boss.png");
	_node->setPosition(p1);
	parent->addChild(_node, 10000);
}

Boss::~Boss()
{
	if (_node)
	{
		_node->removeFromPhysicsWorld();
		_node->removeAllChildren();
		_node->removeFromParentAndCleanup(true);
		_node = nullptr;
	}
}

bool Boss::update(Level* level, float dt)
{
	// Base
	if (Opponent::update(level, dt))
	{
		// send event to the parent chunk that boss has been killed
		((BossFight*)_parent)->onBossKilled();

		// Let it die
		return true;
	}

	// Cache data
	auto player = Game::getInstance()->getPlayer();
	Vec2 playerPos = player->getPosition();
	Vec2 parentPos = _parent->getPosition();
	Vec2 pos = _node->getPosition();
	Vec2 posWS = pos + parentPos;

	// Check if can 'see' a player
	//const int opponentViewRange = 700;
	//bool seePlayer = posWS.distance(playerPos) < opponentViewRange;
	//_state = seePlayer ? State::FollowingPlayer : State::SearchForPlayer;

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

	}

	return false;
}
