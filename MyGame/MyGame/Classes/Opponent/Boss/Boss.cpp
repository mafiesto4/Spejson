#include <string>
#include "cocos2d.h"
#include "Opponent\Opponent.h"
#include "Boss.h"
#include "Game.h"
#include "../../Levels/Chunk.h"
#include "../../Levels/ChunkTypes/BossFight.h"

using namespace std;
using namespace cocos2d;

#define BOSS_SCALE 4

int Bosses = 0;

Boss::Boss(Chunk* parent, Vec2 p1, Vec2 p2)
	:Opponent(parent, 300),
	_p1(p1),
	_p2(p2),
	_sleep(0)
{
	_node = Sprite::create("Textures/boss1.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Textures/boss.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("Textures/bossA.plist");
	startAnim();
	_node->setPosition(p1);
	_node->setScale(BOSS_SCALE);
	parent->addChild(_node, 10000);

	Bosses++;
}

Boss::~Boss()
{
	Bosses--;
}

void Boss::startAnim()
{
	auto anim = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("bossA")));
	anim->setTag(1);
	_node->runAction(anim);
}

bool Boss::update(Level* level, float dt)
{
	// Base
	if (Opponent::update(level, dt))
	{
		static_cast<BossFight*>(_parent)->onBossKilled();
		return true;
	}
	if (Opponent::postUpdate(dt))
	{
		_state = State::Undefined;
		_node->stopAllActions();
		return false;
	}
	if (_node->getActionByTag(1) == nullptr)
		startAnim();

	// Cache data
	auto player = Game::getInstance()->getPlayer();
	Vec2 playerPosWS = player->getPosition();
	Vec2 chunkPosWS = _parent->getPosition();
	auto playerPosCS = playerPosWS - chunkPosWS;// player position in Chunk Space
	Vec2 pos = _node->getPosition();
	Vec2 posWS = pos + chunkPosWS;

	// Update state
	const float epsilon = 1.5f * dt * 100.0f;
	const float sleepEpsilon = 0.001f;
	bool seePlayer = posWS.y - 200 <= playerPosWS.y;
	Vec2 dir = _target - pos;
	bool isOverTargetPoint = abs(dir.x - epsilon) <= epsilon && abs(dir.y - epsilon) <= epsilon;
	float speed = 1.6f;
	if (!seePlayer &&
		_state != GoToA &&
		_state != GoToB &&
		_state != PatrollingB &&
		_state != PatrollingA)
	{
		_state = Undefined;
	}
	switch (_state)
	{
		case Undefined:
			pos = _p1;
			_target = _p2;
			_state = PatrollingB;
			_node->setScaleX(-BOSS_SCALE);
			return false;

		case PatrollingA:
			if (seePlayer)
			{
				if (playerPosCS.x > pos.x)
				{
					_target = _p2;
					_node->setScaleX(-BOSS_SCALE);
				}
				else
				{
					_target = _p1;
					_node->setScaleX(BOSS_SCALE);
				}
				_state = AttackPlayer;
			}
			else if (isOverTargetPoint)
			{
				_target = _p2;
				_state = PatrollingB;
				_node->setScaleX(-BOSS_SCALE);
			}
			break;

		case GoToA:
			if (isOverTargetPoint)
			{
				_target = _p2;
				_state = PatrollingB;
				_node->setScaleX(-BOSS_SCALE);
			}
			break;

		case PatrollingB:
			if (seePlayer)
			{
				if (playerPosCS.x > pos.x)
				{
					_target = _p2;
					_node->setScaleX(-BOSS_SCALE);
				}
				else
				{
					_target = _p1;
					_node->setScaleX(BOSS_SCALE);
				}
				_state = AttackPlayer;
			}
			else if (isOverTargetPoint)
			{
				_target = _p1;
				_state = PatrollingA;
				_node->setScaleX(BOSS_SCALE);
			}
			break;

		case GoToB:
			if (isOverTargetPoint)
			{
				_target = _p1;
				_state = PatrollingA;
				_node->setScaleX(BOSS_SCALE);
			}
			break;

		case AttackPlayer:
			speed = 3;
			if (isOverTargetPoint && abs(_sleep - sleepEpsilon) <= sleepEpsilon)
			{
				// Og³uszenie po ataku xd
				_sleep = 1;
			}
			else if (isOverTargetPoint)
			{
				// Check if it's closer to A or B
				if (playerPosCS.x < (_p1.x + _p2.x) * 0.5f)
				{
					_target = _p2;
					_state = GoToB;
					_node->setScaleX(-BOSS_SCALE);
				}
				else
				{
					_target = _p1;
					_state = GoToA;
					_node->setScaleX(BOSS_SCALE);
				}
			}
			break;
	}

	// Check if is sleeping
	if (_sleep <= sleepEpsilon)
	{
		// Update position
		dir.normalize();
		_node->setPosition(pos + dir * speed * dt * 200.0f);
	}
	else
	{
		_sleep -= dt;
		if (_sleep <= sleepEpsilon)
		{
			_sleep = 0;
		}
	}

	return false;
}
