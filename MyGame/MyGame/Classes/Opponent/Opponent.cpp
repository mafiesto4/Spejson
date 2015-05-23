#include "cocos2d.h"
#include "Opponent\Opponent.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "../HUD/DebugGUI.h"
#include "../Levels/Chunk.h"
#include "../Player/Player.h"

using namespace std;
using namespace cocos2d;

Opponent::Opponent(Chunk* parent, int hp)
	:Entity(parent),
	_hp(hp),
	_node(nullptr),
	_state(State::Undefined),
	_ogluszony(false)
{
}

Opponent::~Opponent()
{
	if (_node)
	{
		_node->removeFromPhysicsWorld();
		_node->removeAllChildren();
		_node->removeFromParentAndCleanup(true);
		_node = nullptr;
	}
}

bool Opponent::preUpdate(float dt)
{
	if (_ogluszony > 0)
	{
		_ogluszony -= dt;
		if (_ogluszony <= 0)
		{
			_ogluszony = 0;
			_node->setColor(Color3B(255, 255, 255));
			return false;
		}
		_node->setColor(Color3B(0, 162, 232));
		return true;
	}
	return false;
}

bool Opponent::update(Level* level, float dt)
{
	// Cache data
	auto player = Game::getInstance()->getPlayer();
	Rect playerBox = player->getBox();
	Rect box = _node->getBoundingBox();
	Vec2 parentPos = _parent->getPosition();

	// Check if any bullet shot by player hist the enemy
	for (int i = 0; i < level->_bullets.size(); i++)
	{
		Bullet b = level->_bullets[i];

		// Check damage
		if (b.ShotByPlayer)
		{
			Vec2 pos = b.Node->getPosition() - parentPos;
			if (box.containsPoint(pos))
			{
				// apply stunt
				if (b.Luj)
					_ogluszony = 2.0f;

				// apply damage to the opponent
				onDamage(b.Damage);

				// mark bullet to delete
				b.DistanceLeft = 0;

				// Store bullet state
				level->_bullets[i] = b;

				// Back
				break;
			}
		}
	}

	// Check if has any hp
	if (_hp <= 0)
	{
		// Killed by death
		_parent->addCoin(box.origin + Vec2(0, 64));
		return true;
	}

	// obrażenia jakie otrzymuje player od przeciwników
#if !GOD_MODE
	box.origin += parentPos;
	if (playerBox.intersectsRect(box))
	{
		if (player->getImmune() == false)
		{
			player->applyDamage(20);
			player->setImmune();
			player->onDamage(box.origin.x > playerBox.origin.x);
		}
	}
#endif

	return false;
}
