
#include "Level.h"
#include "..\Player\Player.h"
#include "..\Player\Weapons\Pistol\Pistol.h"
#include "..\Opponent\Alien1\Alien1.h"
#include "HUD\GameHUD.h"
#include "Game.h"
#include "HUD\DebugGUI.h"
#include "Utilities.h"

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
	physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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

// on "init" you need to initialize your instance
bool Level::init()
{
	// init randy srandy dupy blade
	srand(time(NULL));

	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(20, 20, 250, 255)))
	{
		return false;
	}

	// uruchom funckje update(...)
	scheduleUpdateWithPriority(1410);

	// spawn simple floor
	/*for (int i = -10; i < 20; i++)
	{
		addBrick1(Point(i * 64, -20));
	}*/

	// Setup chunks
	setupInitialMap();

	//długa podłoga z jednego kloca(fizyka się nie krztusi) m8
	//addBrick1(Point(0, 0));

	// Create camera
	_camera = Camera::create();
	addChild(_camera);

	// setup player
	auto game = Game::getInstance();
	auto player = game->getPlayer();
	player->setupForLevel(this, Vec2(200, 54));

	// new way to enable touch
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Level::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Level::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//dodaj przeciwnika
	_opponents.push_back(new Alien1("janek", *this));

	// stworz Head Up Display
	_hud = GameHUD::create();
	this->addChild(_hud, 1939);

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

	// update przeciwnikow
	for (int i = 0; i < _opponents.size(); i++)
	{
		_opponents[i]->update(dt);
	}

	// Update bullets
	Rect playerBox = player->getBox();
	int i;
	for (i = 0; i < _bullets.size(); i++)
	{
		Bullet b = _bullets[i];
		
		// Update position
		float distanceToMove = b.Speed * (dt * 1000.0f);
		b.DistanceLeft -= distanceToMove;
		b.Position += b.Direction * distanceToMove;

		// Check damage
		if (b.ShotByPlayer)
		{
			for (int i = 0; i < _opponents.size(); i++)
			{
				auto o = _opponents[i];
				if (o->getBox().containsPoint(b.Position))
				{
					o->onDamage(b.Damage); // apply damage to the opponent
					b.DistanceLeft = 0; // mark bullet to delete
				}
			}
		}
		// Collision vs player
		else if (!b.ShotByPlayer && playerBox.containsPoint(b.Position))
		{
			// Applu damage to the player
			player->applyDamage(b.Damage);
			b.DistanceLeft = 0; // mark bullet to delete
		}

		_bullets[i] = b;
	}

	// Remove old bullets
	i = 0;
	while (i < _bullets.size() && _bullets.size() > 0)
	{
		if (_bullets[i].DistanceLeft <= 0)
		{
			_bullets.erase(_bullets.begin() + i);
			i--;
		}
		i++;
	}

	// Remove killed opponents
	i = 0;
	while (i < _opponents.size() && _opponents.size() > 0)
	{
		if (_opponents[i]->getHP() <= 0)
		{
			delete _opponents[i];
			_opponents.erase(_opponents.begin() + i);
			i--;
		}
		i++;
	}

	// Update camera
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

	// Update chunks
	flushChunks();

	DebugGUI::setVal(2, "Bullets count", _bullets.size());
}

void Level::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	//LayerColor::draw(renderer, transform, flags);

	//DrawPrimitives::drawSolidRect(Vec2::ZERO, getContentSize(), Color4F(getColor()));

	// Draw bullets
	DrawPrimitives::setPointSize(30.0f);
	for (std::vector<Bullet>::iterator it = _bullets.begin(); it != _bullets.end(); ++it)
	{
		DrawPrimitives::drawPoint(it->Position);
	}
}

bool Level::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void Level::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto location = touch->getLocation();

	//addBrick1(location);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto player = Game::getInstance()->getPlayer();
	auto playerPos = player->getPosition();
	auto camPos = _camera->getPosition();

	Bullet bullet;
	bullet.Damage = 10;
	bullet.Direction = location - (playerPos - camPos + visibleSize * 0.5);
	bullet.Position = playerPos;
	bullet.DistanceLeft = 10000;
	bullet.ShotByPlayer = true;
	bullet.Speed = 1;

	shoot(bullet);
}

void Level::addBrick1(Point p)
{
	auto sprite = Sprite::create("Textures/brick1.png");
	sprite->setTag(PHYSICS_TAG_GROUND);
	auto body = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.52f));// PhysicsMaterial(3.0f, 1.0f, 0.6f));
	
	//body->setLinearDamping(0);

	body->setDynamic(false);
	sprite->setPhysicsBody(body);
	sprite->setScaleX(100.0);
	sprite->setPosition(p);
	sprite->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
	this->addChild(sprite);
}

void Level::shoot(Bullet& bullet)
{
	bullet.Direction.normalize();
	_bullets.push_back(bullet);
}


void Level::menuCloseCallback(Object* pSender)
{
	if (m_world->getDebugDrawMask() != PhysicsWorld::DEBUGDRAW_NONE)
	{
		m_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
	}
	else
	{
		m_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	}
}
