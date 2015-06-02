#include <string>
#include "cocos2d.h"
#include "Opponent\Opponent.h"
#include "Opponent\Alien1\Alien1.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "../../Levels/Chunk.h"

using namespace std;
using namespace cocos2d;

bool onContactBegin(PhysicsContact&);

Alien1::Alien1(Chunk* parent, Vec2 p1, Vec2 p2)
	:Opponent(parent),
	_p1(p1 + Vec2(0, 16)),
	_p2(p2 + Vec2(0, 16))
{
	_node = Sprite::create("Textures/walking1.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Textures/walking.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("Textures/walkingA.plist");
	_node->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("walkingA"))));
	_node->setPosition(p1);
	parent->addChild(_node);
}

Alien1::~Alien1()
{
}

bool Alien1::update(Level* level, float dt)
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
		case State::PatrollingB:
		{
			bool isA = _state == State::PatrollingA;

			auto action = _node->getActionByTag(0);
			if (!action)
			{
				if (isA)
				{
					auto anim = MoveTo::create(calMoveDuration(), _p2);
					anim->setTag(0);
					_node->runAction(anim);
					_node->setScaleX(-1);
					_state = State::PatrollingB;
				}
				else
				{
					auto anim = MoveTo::create(calMoveDuration(), _p1);
					anim->setTag(0);
					_node->runAction(anim);
					_node->setScaleX(1);
					_state = State::PatrollingA;
				}
			}
		}
		break;

		default: assert(0);
	}

	return false;
}
