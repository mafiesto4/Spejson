
#include "Level1.h"
#include "..\Player\Player.h"
#include "..\Player\Weapons\Pistol\Pistol.h"
#include "..\Opponent\Alien1\Alien1.h"
#include "HUD\GameHUD.h"
#include "Game.h"
#include "HUD\DebugGUI.h"
#include "Utilities.h"

using namespace cocos2d;

Level1::~Level1()
{
	//CC_SAFE_RELEASE_NULL(_physics);
	//CC_SAFE_RELEASE_NULL(_hud);
	//delete przeciwnik;
}

Scene* Level1::createScene()
{
	// 'scene' is an autorelease object
	//add physicals
	auto scene = Scene::createWithPhysics();

	auto physicsWorld = scene->getPhysicsWorld();
	physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	physicsWorld->setGravity(Vec2(0, -150.0f));
	physicsWorld->setSpeed(2.0f);


	Size visibleSize = Director::getInstance()->getVisibleSize();
	/*
	auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	edgeNode->setPhysicsBody(body);
	scene->addChild(edgeNode);
	*/
	// 'layer' is an autorelease object
	auto layer = Level1::create();
	layer->setPhyWorld(scene->getPhysicsWorld());

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Level1::init()
{
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(20, 20, 250, 255)))
	{
		return false;
	}

	// uruchom funckje update(...)
	this->scheduleUpdateWithPriority(1410);

	// spawn simple floor
	for (int i = -10; i < 20; i++)
	{
		addBrick1(Point(i * 64, -20));
	}

	// setup player
	auto game = Game::getInstance();
	auto player = game->getPlayer();
	player->setupForLevel(this);

	// new way to enable touch
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Level1::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Level1::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//dodaj przeciwnika
	_opponents.push_back(new Alien1("kutasiarz", *this));

	// stworz Head Up Display
	_hud = GameHUD::create();
	addChild(_hud);

	// player follow
	Point origin = Director::sharedDirector()->getVisibleOrigin();
	Size size = Director::sharedDirector()->getVisibleSize();  //default screen size (or design resolution size, if you are using design resolution)
	Point center = Point(size.width / 2 + origin.x, size.height / 2 + origin.y);
	float playfield_width = size.width * 2.0; // make the x-boundry 2 times the screen width
	float playfield_height = size.height * 2.0; // make the y-boundry 2 times the screen height
	//_hud->runAction(Follow::create(player->getNode(), Rect(center.x - playfield_width / 2, center.y - playfield_height / 2, playfield_width, playfield_height)));
	//this->runAction(Follow::create(player->getNode()));

	// Create camera
	_camera = Camera::create();
	addChild(_camera);

	return true;
}

void Level1::update(float dt)
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
		_hud->setPosition(Vec2::ZERO);

		// dump velocity to create smooth effect
		_camVelocity *= powf(CAM_VELOCITY_DUMP, dt);
	}



	DebugGUI::setVal(2, "Bullets count", _bullets.size());
}

void Level1::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	//LayerColor::draw(renderer, transform, flags);

	DrawPrimitives::drawSolidRect(Vec2::ZERO, getContentSize(), Color4F(getColor()));


	// Draw bullets
	DrawPrimitives::setPointSize(30.0f);
	for (std::vector<Bullet>::iterator it = _bullets.begin(); it != _bullets.end(); ++it)
	{
		DrawPrimitives::drawPoint(it->Position);
	}
}

bool Level1::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void Level1::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto location = touch->getLocation();


	//addBrick1(location);

	auto player = Game::getInstance()->getPlayer();
	auto playerPos = player->getPosition();
	
	Bullet bullet;
	bullet.Damage = 10;
	bullet.Direction = location - playerPos;
	bullet.Position = playerPos;
	bullet.DistanceLeft = 10000;
	bullet.ShotByPlayer = true;
	bullet.Speed = 1;

	shoot(bullet);
}

void Level1::addBrick1(Point p)
{
	auto sprite = Sprite::create("Textures/brick1.png");
	sprite->setTag(PHYSICS_TAG_GROUND);
	auto body = PhysicsBody::createBox(sprite->getContentSize());
	body->setDynamic(false);
	sprite->setPhysicsBody(body);
	sprite->setPosition(p);
	sprite->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
	this->addChild(sprite);
}

void Level1::shoot(Bullet& bullet)
{
	bullet.Direction.normalize();
	_bullets.push_back(bullet);
}


void Level1::menuCloseCallback(Object* pSender)
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
