
#include <string>
#include "cocos2d.h"
#include "Player.h"
#include "Game.h"

#include "Player\Weapons\Pistol\Pistol.h"
#include "Player\Weapons\MachineGun\MachineGun.h"
#include "Player\Weapons\Freezer\Freezer.h"

#include "../Objects/Shop.h"
#include "../Objects/Particles/BloodHit.h"

#include "../Levels/Chunk.h"

#include <SimpleAudioEngine.h>
#include <Utilities.h>

using namespace std;
using namespace cocos2d;

#define PLAYER_MOVEMENT_COEFF 100
const float moveSpeed = 50 * PLAYER_MOVEMENT_COEFF;

// forward declaration
bool onContactBegin(PhysicsContact&);

Player::Player(string name)
	:_hp(100),
	_name(name),
	_image(nullptr),
	_body(nullptr),
	_keyboard(nullptr),
	_mouse(nullptr),
	_level(nullptr),
	_maxHeight(100),

	_isPressingW(false),
	_isPressingS(false),
	_isPressingA(false),
	_isPressingD(false),

	_laddered(false),
	_immune(false),
	_time(0),
	_damageIntensity(0),

	_rightDirection(true),
	_isUsingLadder(false),
	_selectedGun(nullptr),
	fireRate(1),
	lifes(1),

	currentState(IDLE),
	stateToBe(IDLE),

	OverLadder(false)
{
#if USE_FREE_CAM
	_useBoost = false;
#endif
	_wantsJump = false;
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
		_mouse->onMouseDown = CC_CALLBACK_1(Player::onMouseDown, this);
		_mouse->onMouseUp = CC_CALLBACK_1(Player::onMouseUp, this);
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
		//deklaracja animacji
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Textures/idle.plist");
		AnimationCache::getInstance()->addAnimationsWithFile("Textures/idleA.plist");
		//movement
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Textures/move.plist");
		AnimationCache::getInstance()->addAnimationsWithFile("Textures/moveA.plist");
		//jump
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Textures/jump.plist");
		AnimationCache::getInstance()->addAnimationsWithFile("Textures/jumpA.plist");

		//fall
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Textures/falling.plist");
		AnimationCache::getInstance()->addAnimationsWithFile("Textures/fallingA.plist");


		_image->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("moveA"))));
		_image->setTag(PHYSICS_TAG_PLAYER);


		//deklaracja animacji
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Textures/idle.plist");
		AnimationCache::getInstance()->addAnimationsWithFile("Textures/idleA.plist");
		//movement
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Textures/move.plist");
		AnimationCache::getInstance()->addAnimationsWithFile("Textures/moveA.plist");
		//jump
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Textures/jump.plist");
		AnimationCache::getInstance()->addAnimationsWithFile("Textures/jumpA.plist");

		//fall
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Textures/falling.plist");
		AnimationCache::getInstance()->addAnimationsWithFile("Textures/fallingA.plist");

		_image->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("moveA"))));




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
		
#endif
	}

	// Clear state
	_wantsJump = false;
	_isPressingA = _isPressingD = _isPressingS = _isPressingW = false;
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

void Player::onPickupAmmo()
{
	_weapons[1]->addAmmo(40);
	_weapons[2]->addAmmo(3);
}

void Player::markLadderUse()
{
	_isUsingLadder = true;
}

void Player::update(float dt)
{
	// Check if grounded or is in the air
	auto pos = _image->getPosition();
	auto size = _image->getContentSize();
	float playerBotom = pos.y - size.height;
	_grounded = playerBotom < 10 || OverLadder;
	OverLadder = false;
	if (!_grounded)
	{
		Chunk* chunk = _level->chunkAtPoint(Vec2(pos.x, playerBotom - (GROUNDED_EPSOLON / 2)));
		if (chunk)
		{
			// Check collsion with all the platforms
			Vec2 chunkRoot = chunk->getPosition();
			for (int i = 0; i < chunk->_platforms.Count(); i++)
			{
				auto platform = chunk->_platforms[i];
				auto platformSize = platform->getContentSize();
				auto platformPos = chunkRoot + platform->getPosition();

				if (abs(platformPos.y - playerBotom) <= GROUNDED_EPSOLON && abs(pos.x - platformPos.x) <= (platformSize.width + size.width) / 2)
				{
					_grounded = true;
					break;
				}
			}
		}
	}
	if (_grounded)
	{
		_doubleJumpFlag = true;
	}

#if USE_FREE_CAM
	Vec2 move = Vec2::ZERO;
	if (_isPressingW)
		move += Vec2(0, 1);
	if (_isPressingS)
		move += Vec2(0, -1);
	if (_isPressingA)
	{
		_image->setScaleX(-1);
		move += Vec2(-1, 0);
	}
	if (_isPressingD)
	{
		_image->setScaleX(1);
		move += Vec2(1, 0);
	}
	move *= _useBoost ? 30 : 5;
	_image->setPosition(pos + move);
#else
	// Check if player is using a ladder
	_body->setGravityEnable(!_isUsingLadder);
	if (_isUsingLadder)
	{
		// disable velocity
		_body->setVelocity(Vec2::ZERO);

		Vec2 move = Vec2::ZERO;
		if (_isPressingW)
		{
			move += Vec2(0, 1);
		}
		if (_isPressingS)
		{
			move += Vec2(0, -1);
		}
		if (_isPressingA)
		{
			_image->setScaleX(-1);
			move += Vec2(-1, 0);
		}
		if (_isPressingD)
		{
			_image->setScaleX(1);
			move += Vec2(1, 0);
		}
		move *= 6;
		_image->setPosition(pos + move);

		// Clear flag
		_isUsingLadder = false;
	}
	else
	{
		// Check if player wants to move
		if (_wantsJump || _isPressingA || _isPressingD)
		{
			auto currentVelocity = _body->getVelocity();
			Vec2 impulse(0.0f, 0.0f);

			// Create impulse direction
			const float jumpSpeed = 4000 * PLAYER_MOVEMENT_COEFF;
			if (_wantsJump && (_grounded || _doubleJumpFlag))
			{
				impulse.y = jumpSpeed;
				_wantsJump = false;
				_doubleJumpFlag = _grounded;
			}
			if (_isPressingA)
			{
				impulse.x = -moveSpeed;
				_image->setScaleX(-1);
			}
			if (_isPressingD)
			{
				impulse.x = moveSpeed;
				_image->setScaleX(1);
			}
			_body->applyImpulse(impulse, _body->getFirstShape()->getCenter());
		}
	}
#endif

	// update broni
	if (_selectedGun)
	{
		_selectedGun->update(dt);
	}

	// timer niezniszczalnosci
	if (_immune)
	{
		_time += dt;
		if (_time > 3)
		{
			_immune = false;
			_time = 0;
		}
	}

	// liczenie pkt za wysokosc
	float h = _image->getPositionY();
	if (_maxHeight < h)
	{
		//_score += h - _maxHeight;
		_maxHeight = h;
	}

	// Check player move direction
	_isMovingUp = _prevPos.y < pos.y;
	_prevPos = _image->getPosition();


	////// Maszyna Stanow (usa machine)

	if (_grounded && !_isPressingA && !_isPressingD)
	{
		stateToBe = IDLE;
	}
	else
	{
		stateToBe = FALLING;
	}

	if (_isPressingA && _grounded) stateToBe = MOVE;
	if (_isPressingD && _grounded) stateToBe = MOVE;

	// maszyna stanów animacji 
	if (currentState != stateToBe)
	{
		_image->stopAllActions();
		switch (stateToBe)
		{
			case IDLE:
				_image->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("idleA"))));
				break;
			case MOVE:
				_image->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("moveA"))));
				break;
			case JUMPING:
				_image->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("jumpA"))));
				break;
			case FALLING:
				_image->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("fallingA"))));
				break;

			default: break;


		}
		currentState = stateToBe;
	}

	// update player damage effect
	if (_damageIntensity > 0)
	{
		if (_damageIntensity - dt <= 0)
		{
			_image->setColor(Color3B::WHITE);
		}
		else
		{
			float perc = 1 - clamp(_damageIntensity * _damageIntensity, 0.0f, 1.0f);
			_image->setColor(Color3B(255, 255 * perc, 255 * perc));
		}
		_damageIntensity -= dt;
	}
}

void Player::applyDamage(float damage)
{
#if !USE_FREE_CAM && !GOD_MODE
	_hp -= damage;
	_damageIntensity = 0.8f;
	if (_hp <= 0)
	{
		_hp = 0;
		_level->onPlayerDeath();
		return;
	}
#endif

	// blood hit
	auto bloodHit = BloodHit::createWithTotalParticles(20, Color3B(210, 20, 16), 1.0f, 1.0f);
	bloodHit->setPosition(10, 10);
	_image->addChild(bloodHit, 100);
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

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/Hit.wav");
#endif
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_ESCAPE: Director::getInstance()->end(); break;
		case EventKeyboard::KeyCode::KEY_SPACE:
		case EventKeyboard::KeyCode::KEY_W: _isPressingW = true; _wantsJump = true; _laddered = true; break;
		case EventKeyboard::KeyCode::KEY_A: _rightDirection = false; _isPressingA = true; break;
		case EventKeyboard::KeyCode::KEY_D:_rightDirection = true; _isPressingD = true; break;
		case EventKeyboard::KeyCode::KEY_S: _isPressingS = true; break;
#if USE_FREE_CAM
		case EventKeyboard::KeyCode::KEY_SHIFT: _useBoost = true; break;
#endif
	}
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_A: _isPressingA = false; break;
		case EventKeyboard::KeyCode::KEY_D: _isPressingD = false; break;
		case EventKeyboard::KeyCode::KEY_SPACE:
		case EventKeyboard::KeyCode::KEY_W: _isPressingW = false; _wantsJump = false; _laddered = false; break;
		case EventKeyboard::KeyCode::KEY_S: _isPressingS = false; break;
#if USE_FREE_CAM
		case EventKeyboard::KeyCode::KEY_SHIFT: _useBoost = false; break;
#endif
	}
}

void Player::onMouseDown(Event* event)
{
	// Gather event data
	EventMouse* data = static_cast<EventMouse*>(event);

	// Check if can use gun
	if (!OverLadder && _selectedGun && !AnyShopInUse)
	{
		_selectedGun->onMouseDown(data->getLocation());
	}

	// Get chunk with teh player and fire event for it's entities
	Chunk* chunk = _level->chunkAtPoint(_image->getPosition());
	if (chunk)
	{
		for (int i = 0; i < chunk->_entities.Count(); i++)
		{
			chunk->_entities[i]->onMouseDown(data);
		}
	}
}

void Player::onMouseUp(Event* event)
{
	// Gather event data
	EventMouse* data = static_cast<EventMouse*>(event);

	// Check if can use gun
	if (!OverLadder && _selectedGun && !AnyShopInUse)
	{
		_selectedGun->onMouseUp(data->getLocation());
	}

	// Get chunk with teh player and fire event for it's entities
	Chunk* chunk = _level->chunkAtPoint(_image->getPosition());
	if (chunk)
	{
		for (int i = 0; i < chunk->_entities.Count(); i++)
		{
			chunk->_entities[i]->onMouseUp(data);
		}
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
