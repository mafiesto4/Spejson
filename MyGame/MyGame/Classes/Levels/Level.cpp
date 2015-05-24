
#include "Level.h"
#include "..\Player\Player.h"
#include "..\Player\Weapons\Pistol\Pistol.h"
#include "..\Player\Weapons\Freezer\Freezer.h"
#include "..\Opponent\Alien1\Alien1.h"
#include "HUD\GameHUD.h"
#include "Game.h"
#include "HUD\DebugGUI.h"
#include "Utilities.h"
#include "Chunk.h"

using namespace cocos2d;

Level::~Level()
{
	// Clean chunks
	cleanAllChunks();
}

Scene* Level::createScene()
{
	// Create scene
	auto scene = Scene::createWithPhysics();

	// Setup physics simulation
	auto physicsWorld = scene->getPhysicsWorld();
#if USE_PHYSICS_DEBUG_LAYER
	physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif
	physicsWorld->setGravity(Vec2(0.0f, -300.0f));
	physicsWorld->setSpeed(2.0f);
	physicsWorld->setUpdateRate(1.0f);
	physicsWorld->setSubsteps(4);

	// Create level layer
	auto level = Level::create();
	level->m_world = physicsWorld;
	scene->addChild(level);

	return scene;
}

bool Level::init()
{
	// Init randomizer
	srand(time(NULL));

	// Init base
	if (!Layer::init())
	{
		return false;
	}

	// Enable update function
	scheduleUpdateWithPriority(1410);

	// setup player
	auto game = Game::getInstance();
	auto player = game->getPlayer();
	player->setupForLevel(this, Vec2(200, 54));

	// Setup chunks
	setupInitialMap();

	// Create camera
	_camera = Camera::create();
	addChild(_camera);

	// Enable touch
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Level::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Level::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	// stworz Head Up Display
	_hud = GameHUD::create();
	addChild(_hud, 1939);

	// Create lava
	_lava = Lava::create();
	addChild(_lava);

	return true;
}

void Level::update(float dt)
{
	auto game = Game::getInstance();
	if (!game)
		return;

	auto player = game->getPlayer();
	if (!player)
		return;

	// Update player
	player->update(dt);
	
	// Update bullets
	Rect playerBox = player->getBox();
	int i;
	for (i = 0; i < _bullets.size(); i++)
	{
		Bullet b = _bullets[i];

		// Update position
		float distanceToMove = b.Speed * (dt * 1000.0f);
		b.DistanceLeft -= distanceToMove;
		Vec2 pos = b.Node->getPosition() + b.Direction * distanceToMove;

		// Chunk at point
		Chunk* chunk = chunkAtPoint(pos);
		if (chunk)
		{
			// Platform at point
			Sprite* platform = chunk->platformAtPoint(pos);
			if (platform)
			{
				// Mark bullet to delete
				b.DistanceLeft = 0;
			}
		}

		// Collision vs player
		if (!b.ShotByPlayer && playerBox.containsPoint(pos))
		{
			// Applu damage to the player
			player->applyDamage(b.Damage);
			b.DistanceLeft = 0; // mark bullet to delete
		}

		// Change position
		b.Node->setPosition(pos);

		_bullets[i] = b;
	}

	// Remove old bullets
	i = 0;
	while (i < _bullets.size() && _bullets.size() > 0)
	{
		if (_bullets[i].DistanceLeft <= 0)
		{
			auto node = _bullets[i].Node;

			node->removeFromParent();

			_bullets.erase(_bullets.begin() + i);
			i--;
		}
		i++;
	}

	// Update camera
#if USE_FREE_CAM
	{
		// move camera and HUD
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 newPos =  player->getPosition();
		_camera->setPosition(newPos);
		_hud->setPosition(newPos - visibleSize * 0.5f);
	}
#else
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 targetPos = player->getPosition();// +Vec2(visibleSize.width * 0.3f, visibleSize.height * 0.1f);
		Vec2 prevCamPos = _camera->getPosition();

		_camVelocity += (targetPos - prevCamPos) * CAM_ACCEL;

		float velocitySpeed = clamp(_camVelocity.length(), 0.0f, CAM_MAX_VELOCITY);
		if (velocitySpeed > 0.0f)
		{
			_camVelocity.normalize();
			_camVelocity *= velocitySpeed;
		}

		// move camera and HUD
		Vec2 newPos = prevCamPos + _camVelocity * dt;
		_camera->setPosition(newPos);
		_hud->setPosition(newPos - visibleSize * 0.5f);

		// dump velocity to create smooth effect
		_camVelocity *= powf(CAM_VELOCITY_DUMP, dt);
	}
#endif

	// Update chunks
	updateChunks(dt);

	DebugGUI::setVal(4, "Money", player->Cash());
	DebugGUI::setVal(2, "Bullets count", _bullets.size());
}

bool Level::onTouchBegan(Touch *touch, Event *unused_event)
{
	auto player = Game::getInstance()->getPlayer();
	auto location = touch->getLocation();

	auto gun = player->getGun();
	if (gun)
	{
		gun->onMouseDown(location);
	}

	return true;
}

void Level::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto player = Game::getInstance()->getPlayer();
	auto location = touch->getLocation();

	auto gun = player->getGun();
	if (gun)
	{
		gun->onMouseUp(location);
	}
}

void Level::addBullet(Bullet& bullet)
{
	bullet.Direction.normalize();
	_bullets.push_back(bullet);
	addChild(bullet.Node);
}
