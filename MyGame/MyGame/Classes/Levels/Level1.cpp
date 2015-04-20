
#include "Level1.h"
#include "..\Player\Player.h"
#include "..\Player\Weapons\Pistol\Pistol.h"
#include "..\Opponent\Alien1\Alien1.h"
#include "HUD\GameHUD.h"
#include "Game.h"
#include "HUD\DebugGUI.h"

using namespace cocos2d;

//test
Alien1 *przeciwnik;



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

	auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	edgeNode->setPhysicsBody(body);
	scene->addChild(edgeNode);

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
	this->scheduleUpdate();

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
	przeciwnik = new Alien1("kutasiarz", *this);

	// stworz Head Up Display
	_hud = GameHUD::create();
	addChild(_hud);

	return true;
}

void Level1::update(float dt)
{
	// Base
	//Level1::update(dt);

	// Update player
	auto game = Game::getInstance();
	if (game)
	{
		auto player = game->getPlayer();
		if (player)
		{
			player->update(dt);
		}

		if (przeciwnik)
			przeciwnik->update(dt);
	}

	// Update bullets
	int i;
	for (i = 0; i < _bullets.size(); i++)
	{
		Bullet b = _bullets[i];
		
		// Update position
		float distanceToMove = b.Speed * (dt * 1000.0f);
		b.DistanceLeft -= distanceToMove;
		b.Position += b.Direction * distanceToMove;

		// Check damage


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
	sprite->setTag(1);
	auto body = PhysicsBody::createBox(sprite->getContentSize());
	body->setDynamic(false);
	sprite->setPhysicsBody(body);
	sprite->setPosition(p);
	this->addChild(sprite);
}

void Level1::shoot(Bullet& bullet)
{
	bullet.Direction.normalize();
	_bullets.push_back(bullet);
}

void Level1::addNewSpriteAtPosition(Point p)
{
	auto sprite = Sprite::create("Textures/twitter.png");
	sprite->setTag(1);
	auto body = PhysicsBody::createCircle(sprite->getContentSize().width / 2);
	sprite->setPhysicsBody(body);
	sprite->setPosition(p);
	this->addChild(sprite);
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
