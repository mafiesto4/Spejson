
#include "Level.h"
#include "..\Player\Player.h"
#include "HUD\GameHUD.h"
#include "Game.h"
#include "Utilities.h"
#include "Chunk.h"
#include "../Scores/Highscores.h"
#include "Background.h"
#include "../Objects/Shop.h"
#include <SimpleAudioEngine.h>
#include <MainMenu/DeadScreen.h>

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
	auto level = create();
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

	// Create background
	_background = Background::create(this);

	// setup player
	auto game = Game::getInstance();
	auto player = game->getPlayer();
	player->setupForLevel(this, Vec2(200, 54));

	// Setup chunks
	setupInitialMap();

	// Create camera
	_camera = Camera::create();
	addChild(_camera);

	// stworz Head Up Display
	_hud = GameHUD::create();
	addChild(_hud, 1939);

	// Create lava
	_lava = Lava::create();
	addChild(_lava, 10);

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

	AnyShopInUse = false;

	// Update player
	player->update(dt);
	
	// Update bullets
	Rect playerBox = player->getBox();
	unsigned int i;
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
		if (!b.ShotByPlayer)
		{
			Rect bulletBox = Rect(pos.x - (BULLET_SIZE / 2), pos.y - (BULLET_SIZE / 2), BULLET_SIZE, BULLET_SIZE);
			if (playerBox.intersectsRect(bulletBox))
			{
				// Applu damage to the player
				player->applyDamage(b.Damage);
				b.DistanceLeft = 0; // mark bullet to delete
			}
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
		auto hudPos = newPos - visibleSize * 0.5f;
		_hud->setPosition(hudPos);
		_background->setPosition(hudPos);
	}
#else
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 targetPos = player->getPosition();
		targetPos.x = clamp(targetPos.x, visibleSize.width * 0.4f, CHUNKS_WIDTH - visibleSize.width * 0.4f);
		targetPos.y = max(targetPos.y, visibleSize.height * 0.4f);
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
		Vec2 guiPos = newPos - visibleSize * 0.5f;
		_hud->setPosition(guiPos);
		_background->setPosition(guiPos);

		// dump velocity to create smooth effect
		_camVelocity *= powf(CAM_VELOCITY_DUMP, dt);
	}
#endif

	// Update chunks
	updateChunks(dt);
}

void Level::onPlayerDeath()
{
	// Save player result
	int score = Game::getInstance()->getPlayer()->getScore();
	Scores.addRecord("player1", score);
	Scores.save();

	// Show dead screen
	Director::getInstance()->replaceScene(DeadScreen::create(score));
}

void Level::addBullet(Bullet& bullet)
{
	bullet.Direction.normalize();
	_bullets.push_back(bullet);
	addChild(bullet.Node);
}
