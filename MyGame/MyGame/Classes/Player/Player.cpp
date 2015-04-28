
#include <string>
#include "cocos2d.h"
#include "Player.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Player\Weapons\Pistol\Pistol.h"
#include "../HUD/DebugGUI.h"


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

void Player::setupForLevel(Level* level, Vec2 spawnPoint)
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
		_image->setPosition(spawnPoint);
	}
	else
	{
		// Create player sprtie with physics body
		_image = Sprite::create("Textures/pawn1.png");
		_image->setTag(PHYSICS_TAG_PLAYER);
		_body = PhysicsBody::createBox(_image->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.62f));
		_body->setMass(1000);
		//_body->setLinearDamping(0);

		//_body->setGravityEnable(false);
		_image->setPhysicsBody(_body);
		_image->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
		_image->setPosition(spawnPoint);

		// Disable player rotation
		_body->setRotationOffset(0);
		_body->setRotationEnable(false);
		_body->setVelocityLimit(8 * PLAYER_MOVEMENT_COEFF);
		_body->setGravityEnable(true);
		
		// listener dla groundchecka
		auto contactListener = EventListenerPhysicsContact::create();
		contactListener->onContactBegin = CC_CALLBACK_1(Player::onContactBegin, this);
		contactListener->onContactSeperate = CC_CALLBACK_1(Player::onContactSeperate, this);
		eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, level);


		// debug player pos
		playerPosLabel = Label::createWithTTF("", "Fonts/arial.ttf", 24);
		playerPosLabel->setAnchorPoint(Vec2(1, 1));
		playerPosLabel->setPosition(Vec2::ZERO);
		_image->addChild(playerPosLabel);
	}

	// Clear state
	_wantsJump = _wantsMoveLeft = _wantsMoveRight = false;

	// add node to the level
	level->addChild(_image);

	//setup weaopn, kontrolnie xd
	auto bron = new Pistol();
	bron->setupForPlayer(_image);
}

void Player::update(float dt)
{
	// Check if player wants to move
	if ((_wantsJump || _wantsMoveLeft || _wantsMoveRight))
	{
		auto currentVelocity = _body->getVelocity();
		Vec2 impulse(0.0f, 0.0f);

		// Create impulse direction
		const float jumpSpeed = 4000 * PLAYER_MOVEMENT_COEFF;
		const float moveSpeed = 50 * PLAYER_MOVEMENT_COEFF;
		if (_wantsJump)// && _grounded)
		{
			impulse.y = jumpSpeed;
		}
		if (_wantsMoveLeft)
		{
			impulse.x = -moveSpeed;
		}
		if (_wantsMoveRight)
		{
			impulse.x = moveSpeed;
		}
		_body->applyImpulse(impulse, _body->getFirstShape()->getCenter());
	}
	else
	{
		//_body->setVelocity(Vec2(0, 0));
	}

	_wantsJump = false;


	// update player pos debug text
	stringstream text;
	auto pos = _image->getPosition();
	text << "Pos: " << (int)pos.x << ", " << (int)pos.y;
	playerPosLabel->setString(text.str());
}


bool Player::onContactBegin(PhysicsContact& contact)
{
	_grounded = true;
	DebugGUI::setVal(4, "Grounded", _grounded);
	return true;
	//trzeba jeszce pododawaæ tagi w sensie jest grounded jak koliduje z pod³o¿em tylko
}

void Player::onContactSeperate(PhysicsContact& contact)
{
	_grounded = false;
	DebugGUI::setVal(4, "Grounded", _grounded);
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