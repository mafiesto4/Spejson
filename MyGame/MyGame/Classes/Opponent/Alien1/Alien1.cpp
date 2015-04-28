#include <string>
#include "cocos2d.h"
#include "Opponent\Opponent.h"
#include "Opponent\Alien1\Alien1.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "../../HUD/DebugGUI.h"

using namespace std;
using namespace cocos2d;

bool onContactBegin(PhysicsContact&);

Alien1::Alien1(string name, Node& parent) :Opponent(name)
{
	_hp = 100;
	_body = nullptr;


	// Create player sprtie with physics body
	_node = Sprite::create("Textures/twitter.png");
	_node->setTag(PHYSICS_TAG_OPPONENT);
	_body = PhysicsBody::createBox(_node->getContentSize(), PhysicsMaterial(0.17, 0.06, 1.1));
	_node->setPhysicsBody(_body);
	_node->setPosition(Vec2(200, 100));

	// Disable player rotation
	/*_body->setRotationOffset(0);
	_body->setRotationEnable(false);
	_body->setAngularVelocity(0);
	_body->setVelocityLimit(2 * PLAYER_MOVEMENT_COEFF);*/
	parent.addChild(_node);
}

void Alien1::update(float dt)
{
	// Call base
	Opponent::update(dt);


	
	DebugGUI::setVal(3, "Alien HP", _hp);
}

Alien1::~Alien1()
{
	CC_SAFE_RELEASE_NULL(_node);
}

