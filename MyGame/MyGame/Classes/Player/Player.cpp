
#include <string>
#include "cocos2d.h"
#include "Player.h"
#include "Game.h"
#include "Box2D\Box2D.h"

#include "Player\Weapons\Pistol\Pistol.h"
#include "Player\Weapons\MachineGun\MachineGun.h"
#include "Player\Weapons\Freezer\Freezer.h"

#include "../HUD/DebugGUI.h"

using namespace std;
using namespace cocos2d;

#define PLAYER_MOVEMENT_COEFF 100
const float moveSpeed = 50 * PLAYER_MOVEMENT_COEFF;

// forward declaration
bool onContactBegin(PhysicsContact&);

Player::Player(string name)
	:_hp(100),
	_name(name),
	_keyboard(nullptr),
	_mouse(nullptr),
	_image(nullptr),
	_body(nullptr),
	_selectedGun(nullptr),
	_level(nullptr)
{
#if USE_FREE_CAM
	_useBoost = false;
#endif
	_wantsDown = false;
	_wantsJump = false;
	_wantsMoveLeft = false;
	_wantsMoveRight = false;
}

Player::~Player()
{
	CC_SAFE_RELEASE_NULL(_image);
	CC_SAFE_RELEASE_NULL(_keyboard);
}

void Player::setupForLevel(Level* level, Vec2 spawnPoint)
{
	// Cache data
	_level = level;
	auto director = Director::getInstance();
	auto eventDispatcher = director->getEventDispatcher();

	// Input
	if (_keyboard == nullptr)
	{
		_keyboard = EventListenerKeyboard::create();
		_keyboard->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
		_keyboard->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
		eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboard, level);
	}
	if (_mouse == nullptr)
	{
		_mouse = EventListenerMouse::create();
		/*_keyboard->onMouseDown = [](cocos2d::Event* event)
			{
			try
			{
			EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
			mouseEvent->getMouseButton();
			std::stringstream message;
			message << "Mouse event: Button: " << mouseEvent->getMouseButton() << "pressed at point (" <<
			mouseEvent->getLocation().x << "," << mouseEvent->getLocation().y << ")"; MessageBox(message.str().c_str(), "Mouse Event Details");
			}
			catch (std::bad_cast& e)
			{// Not sure what kind of event you passed us cocos, but it was the wrong onereturn;}};
			listener->onMouseMove = [](cocos2d::Event* event)
			{// Cast Event to EventMouse for position details like above
			cocos2d::log("Mouse moved event");
			};
			listener->onMouseUp = [](cocos2d::Event* event)
			{
			cocos2d::log("Mouse button released");
			};
			_eventDispatcher->addEventListenerWithFixedPriority(listener, 1); return true;
			}
			*/

		_mouse->onMouseScroll = CC_CALLBACK_1(Player::onMouseScroll, this);
		eventDispatcher->addEventListenerWithSceneGraphPriority(_mouse, level);
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

#if !USE_FREE_CAM
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
#endif

		// debug player pos
		playerPosLabel = Label::createWithTTF("", "Fonts/arial.ttf", 24);
		playerPosLabel->setAnchorPoint(Vec2(1, 1));
		playerPosLabel->setPosition(Vec2::ZERO);
		_image->addChild(playerPosLabel);
	}

	// Clear state
	_wantsJump = _wantsMoveLeft = _wantsMoveRight = false;
	_score = 0;
	_cash = 0;

	// add node to the level
	level->addChild(_image);

	// Create weapons
	_weapons[0] = new Pistol(level);
	_weapons[1] = new MachineGun(level);
	_weapons[2] = new Freezer(level);

	// Select pistol
	pickupWeapon(Weapon::Type::Pistol);
}

void Player::pickupWeapon(Weapon::Type type)
{
	// Set flag
	_weapons[(int)type]->CanUse = true;

	// Select it
	selectWeapon(type);
}

void Player::selectWeapon(Weapon::Type type)
{
	// Validate
	if (!_weapons[(int)type]->CanUse)
	{
		return;
	}

	// Check if has previous weapon
	if (_selectedGun)
	{
		// Check if selected gun won't change
		if (_selectedGun->getType() == type)
		{
			// Back
			return;
		}

		// Unlink
		_selectedGun->onDeselect();

	}

	// Link
	_selectedGun = _weapons[(int)type];
	_selectedGun->onSelect(_image);
}

void Player::markLadderUse()
{
	_isUsingLadder = true;
}

void Player::update(float dt)
{
#if USE_FREE_CAM
	Vec2 move = Vec2::ZERO;
	if (_wantsJump)
		move += Vec2(0, 1);
	if (_wantsDown)
		move += Vec2(0, -1);
	if (_wantsMoveLeft)
	{
		_image->setScaleX(-1);
		move += Vec2(-1, 0);
	}
	if (_wantsMoveRight)
	{
		_image->setScaleX(1);
		move += Vec2(1, 0);
	}
	move *= _useBoost ? 30 : 10;
	_image->setPosition(_image->getPosition() + move);
#else
	// Check if player is using a ladder
	_body->setGravityEnable(!_isUsingLadder);
	if (_isUsingLadder)
	{
		// disable velocity
		_body->setVelocity(Vec2::ZERO);

		Vec2 move = Vec2::ZERO;
		if (_wantsJump)
			move += Vec2(0, 1);
		if (_wantsDown)
			move += Vec2(0, -1);
		if (_wantsMoveLeft)
		{
			_image->setScaleX(-1);
			move += Vec2(-1, 0);
		}
		if (_wantsMoveRight)
		{
			_image->setScaleX(1);
			move += Vec2(1, 0);
		}
		move *= 6;
		_image->setPosition(_image->getPosition() + move);

		// Clear flag
		_isUsingLadder = false;
	}
	else
	{
		// Check if player wants to move
		if ((_wantsJump || _wantsMoveLeft || _wantsMoveRight))
		{
			auto currentVelocity = _body->getVelocity();
			Vec2 impulse(0.0f, 0.0f);

			// Create impulse direction
			const float jumpSpeed = 4000 * PLAYER_MOVEMENT_COEFF;
			if (_wantsJump)// && _grounded)
			{
				impulse.y = jumpSpeed;
			}
			if (_wantsMoveLeft)
			{
				impulse.x = -moveSpeed;
				_image->setScaleX(-1);
			}
			if (_wantsMoveRight)
			{
				impulse.x = moveSpeed;
				_image->setScaleX(1);
			}
			_body->applyImpulse(impulse, _body->getFirstShape()->getCenter());
		}
		_wantsJump = false;
	}
#endif

	if (_selectedGun)
	{
		_selectedGun->update(dt);
	}

	if (_immune)
	{
		_time += dt;
		if (_time > 3)
		{
			_immune = false;
			_time = 0;
		}
	}

	// update player pos debug text
	stringstream text;
	auto pos = _image->getPosition();
	text << "Pos: " << (int)pos.x << ", " << (int)pos.y;
	playerPosLabel->setString(text.str());

	DebugGUI::setVal(5, "cash", _cash);
}

void Player::onDamage(bool pushRight)
{
#if !USE_FREE_CAM

	// Create impulse direction
	const float moveSpeed = 4000 * PLAYER_MOVEMENT_COEFF;

	Vec2 impulse(0.0f, 0.0f);
	if (pushRight)
	{
		impulse.x = -moveSpeed;
	}
	else
	{
		impulse.x = moveSpeed;
	}
	impulse.y = moveSpeed;
	_body->applyImpulse(impulse, _body->getFirstShape()->getCenter());
#endif
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
		case EventKeyboard::KeyCode::KEY_ESCAPE: Director::getInstance()->end(); break;
		case EventKeyboard::KeyCode::KEY_SPACE:
		case EventKeyboard::KeyCode::KEY_W: _wantsJump = true; _laddered = true;  break;
		case EventKeyboard::KeyCode::KEY_A: _rightDirection = false; _wantsMoveLeft = true; break;
		case EventKeyboard::KeyCode::KEY_D:_rightDirection = true; _wantsMoveRight = true; break;
		case EventKeyboard::KeyCode::KEY_S: _wantsDown = true; break;
#if USE_FREE_CAM
		case EventKeyboard::KeyCode::KEY_SHIFT: _useBoost = true; break;
#endif
	}
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_A: _wantsMoveLeft = false; break;
		case EventKeyboard::KeyCode::KEY_D: _wantsMoveRight = false; break;
		case EventKeyboard::KeyCode::KEY_SPACE:
		case EventKeyboard::KeyCode::KEY_W: _wantsJump = false;  _laddered = false; break;
		case EventKeyboard::KeyCode::KEY_S: _wantsDown = false; break;
#if USE_FREE_CAM
		case EventKeyboard::KeyCode::KEY_SHIFT: _useBoost = false; break;
#endif
	}
}

void Player::onMouseScroll(Event* event)
{
	// Gather event data
	EventMouse* data = (EventMouse*)event;

	// Find firt usable weapon
	int weaponIndex = (int)_selectedGun->getType();
	if (data->getScrollY() > 0)
	{
		do
		{
			weaponIndex--;

			if (weaponIndex < 0)
				weaponIndex = (int)Weapon::Type::MAX - 1;

		} while (!_weapons[weaponIndex]->CanUse);
	}
	else
	{
		do
		{
			weaponIndex++;

			if (weaponIndex >= (int)Weapon::Type::MAX)
				weaponIndex = 0;

		} while (!_weapons[weaponIndex]->CanUse);
	}

	// Select weapon
	selectWeapon((Weapon::Type)weaponIndex);
}
