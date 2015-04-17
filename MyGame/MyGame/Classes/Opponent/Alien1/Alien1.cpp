#include <string>
#include "cocos2d.h"
#include "Opponent\Opponent.h"
#include "Opponent\Alien1\Alien1.h"
#include "Game.h"
#include "Box2D\Box2D.h"

using namespace std;
using namespace cocos2d;

bool onContactBegin(PhysicsContact&);

Alien1::Alien1(string name, Node& parent) :Opponent(name)
{
	_hp = 10;
	_image = nullptr;
	_body = nullptr;
	// Create player sprtie with physics body
	_image = Sprite::create("Textures/twitter.png");
	//_image->setTag(PHYSICS_TAG_PLAYER);
	_body = PhysicsBody::createBox(_image->getContentSize(), PhysicsMaterial(0.17, 0.06, 1.1));
	_image->setPhysicsBody(_body);
	_image->setPosition(Vec2(200, 100));

	// Disable player rotation
	/*_body->setRotationOffset(0);
	_body->setRotationEnable(false);
	_body->setAngularVelocity(0);
	_body->setVelocityLimit(2 * PLAYER_MOVEMENT_COEFF);*/
	parent.addChild(_image);
	
	auto moveTo = MoveTo::create(4, Vec2(50, 10));
	_image->runAction(moveTo);
}

void Alien1::update(float dt)
{
	

}

Alien1::~Alien1()
{
	CC_SAFE_RELEASE_NULL(_image);
}


