#include <string>
#include "cocos2d.h"
#include "Opponent\Opponent.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "../HUD/DebugGUI.h"

using namespace std;
using namespace cocos2d;

Opponent::Opponent(string name)
	:_hp(100),
	_name(name),
	_node(nullptr),
	_body(nullptr),
	_state(State::Undefined)
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

void Opponent::update(float dt)
{
	// check if has image node created
	if (!_node)
		return;

	switch (_state)
	{
		case State::Undefined:
		{
			auto anim = MoveTo::create(calMoveDuration(), Vec2(400, 66));
			anim->setTag(0);
			_node->runAction(anim);

			_state = State::PatrollingA;

			DebugGUI::setVal(0, "Alien state", "Undefined");
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
					auto anim = MoveTo::create(calMoveDuration(), Vec2(1000, 66));
					anim->setTag(0);
					_node->runAction(anim);
					_state = State::PatrollingB;
				}
				else
				{
					auto anim = MoveTo::create(calMoveDuration(), Vec2(400, 66));
					anim->setTag(0);
					_node->runAction(anim);
					_state = State::PatrollingA;
				}
			}

			if (isA)
				DebugGUI::setVal(0, "Alien state", "PatrollingA");
			else
				DebugGUI::setVal(0, "Alien state", "PatrollingB");
		}
		break;

		case State::Shooting:
		{


			DebugGUI::setVal(0, "Alien state", "Shooting");
		}
		break;


		case State::SearchForPlayer:
		{

			DebugGUI::setVal(0, "Alien state", "SearchForPlayer");
		}
		break;

		default: assert(0);
	}

	// debug
	DebugGUI::setVal(1, "Alien pos", _node->getPosition());


}
