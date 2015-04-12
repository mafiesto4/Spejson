
#include <string>
#include "cocos2d.h"
#include "Player.h"
#include "Game.h"
#include "Box2D\Box2D.h"

using namespace std;
using namespace cocos2d;

#define PLAYER_MOVEMENT_COEFF 100

// forward declaration
bool onContactBegin(PhysicsContact&);

Player::Player(string name)
	:_hp(100),
	_name(name),
	_keyboard(nullptr),
	_image(nullptr),
	_body(nullptr)
{

}

Player::~Player()
{
	CC_SAFE_RELEASE_NULL(_image);
	CC_SAFE_RELEASE_NULL(_keyboard);
}

void Player::setupForLevel(Level1* level)
{
	// Cache data
	auto director = Director::getInstance();
	auto eventDispatcher = director->getEventDispatcher();

	// sprawdz czy keyboard listner zstal stworzony
	if (&_keyboard != NULL)
	{
		// Initializing and binding 
		auto keyboard = EventListenerKeyboard::create();
		keyboard->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
		keyboard->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
		eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard, level);
	}

	// Check if created
	if (_image)
	{
		_image->removeFromParent();
	}
	else
	{
		// Create player sprtie with physics body
		_image = Sprite::create("Textures/pawn1.png");
		_image->setTag(PHYSICS_TAG_PLAYER);
		_body = PhysicsBody::createBox(_image->getContentSize(), PhysicsMaterial(0.17, 0.06, 1.1));
		_image->setPhysicsBody(_body);
		_image->setPosition(Vec2(100, 100));

		// Disable player rotation
		_body->setRotationOffset(0);
		_body->setRotationEnable(false);
		_body->setAngularVelocity(0);
		_body->setVelocityLimit(2 * PLAYER_MOVEMENT_COEFF);
		/*
		// add contact event listener
		auto contactListener = EventListenerPhysicsContact::create();
		contactListener->onContactBegin = CC_CALLBACK_1(onContactBegin, _image);
		eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, _image);*/
	}

	// Clear state
	_wantsJump = _wantsMoveLeft = _wantsMoveRight = false;

	// add node to the level
	level->addChild(_image);
}

void Player::update(float dt)
{
	// Check if player wants to move
	if (_wantsJump || _wantsMoveLeft || _wantsMoveRight)
	{
		auto currentVelocity = _body->getVelocity();
		Vec2 impulse(0.0f, 0.0f);

		// Create impulse direction
		const float jumpSpeed = 400 * PLAYER_MOVEMENT_COEFF;
		const float moveSpeed = 10 * PLAYER_MOVEMENT_COEFF;
		bool canMoveRL = true;// fabs(currentVelocity.x) < 2 * PLAYER_MOVEMENT_COEFF;
		if (_wantsJump)
		{
			impulse.y = jumpSpeed;
			_wantsJump = false;
		}
		if (_wantsMoveLeft && canMoveRL)
		{
			impulse.x = -jumpSpeed;
		}
		if (_wantsMoveRight && canMoveRL)
		{
			impulse.x = jumpSpeed;
		}
		_body->applyImpulse(impulse, _body->getFirstShape()->getCenter());
	}
	else
	{
		//_body->setVelocity(Vec2(0, 0));
	}
}

bool onContactBegin(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	/*
	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == PHYSICS_TAG_PLAYER)
		{
			nodeB->removeFromParentAndCleanup(true);
		}
		else if (nodeB->getTag() == PHYSICS_TAG_PLAYER)
		{
			nodeA->removeFromParentAndCleanup(true);
		}
	}
	*/
	// bodies can collide
	return true;
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
		{
			Director::getInstance()->end();
		}
		break;
		case EventKeyboard::KeyCode::KEY_SPACE:
		case EventKeyboard::KeyCode::KEY_W: _wantsJump = true; break;
		case EventKeyboard::KeyCode::KEY_A: _wantsMoveLeft = true; break;
		case EventKeyboard::KeyCode::KEY_D: _wantsMoveRight = true; break;
		default:
			break;
	}
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_A: _wantsMoveLeft = false; break;
		case EventKeyboard::KeyCode::KEY_D: _wantsMoveRight = false; break;
		default:
			break;
	}
}