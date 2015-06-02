#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Objects/Shop.h"
#include "../Levels/Chunk.h"
#include <SimpleAudioEngine.h>

using namespace cocos2d;

bool AnyShopInUse = false;

Shop::Shop(Chunk* parent, Vec2 pos)
	:Entity(parent, pos),
	_isOver(false),
	_animTime(0),
	_image(nullptr),
	_logo(nullptr)
{
	auto level = parent->getLevel();

	// Shop image
	_image = Sprite::create("Textures/shop1.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Textures/shop.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("Textures/shopA.plist");
	_image->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("shopA"))));
	_image->setPosition(pos + Vec2(64, 0));
	_image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_parent->addChild(_image);

	// Shop logo
	_logo = Sprite::create("Textures/shopTitle.jpg");
	_logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_logo->setVisible(false);
	level->addChild(_logo);

	// Shop info
	_info = Sprite::create("Textures/shopInfo.png");
	_info->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_info->setVisible(false);
	level->addChild(_info);

	// Ammo Pack
	_itemAmmo = Sprite::create("Textures/shop_AmmoPack.png");
	_itemAmmo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_itemAmmo->setVisible(false);
	level->addChild(_itemAmmo);

	// Med Pack
	_itemMed = Sprite::create("Textures/shop_MedPack.png");
	_itemMed->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_itemMed->setVisible(false);
	level->addChild(_itemMed);

	// Freezer
	_itemFreezer = Sprite::create("Textures/shop_Freezer.png");
	_itemFreezer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_itemFreezer->setVisible(false);
	level->addChild(_itemFreezer);
}

Shop::~Shop()
{
	if (_image)
	{
		_image->removeAllChildren();
		_image->removeFromParentAndCleanup(true);
		_image = nullptr;
	}
}

bool Shop::update(Level* level, float dt)
{
	// Cache data
	auto player = Game::getInstance()->getPlayer();
	auto playerPos = player->getPosition();
	Vec2 pos = _image->getPosition() + _parent->getPosition() - Vec2(CHUNKS_BLOCK_SIZE_HALF, CHUNKS_BLOCK_SIZE_HALF);
	auto size = _image->getContentSize();
	Rect box = Rect(pos.x, pos.y, size.width, size.height);

	// Test collision
	bool isOver = player->getBox().intersectsRect(box);

	// Check if is over
	if (isOver)
		_animTime += dt;
	else
		_animTime -= dt;
	if (_animTime < 0)
		_animTime = 0;
	else if (_animTime > SHOP_ANIMATION_TIME)
		_animTime = SHOP_ANIMATION_TIME;

	// Check if show shop GUI
	if (_animTime > 0)
	{
		// Cache data
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Size visibleSize2 = visibleSize * 0.5f;
		auto camPos = level->getCamera()->getPosition();
		float procentAnimacji = _animTime / SHOP_ANIMATION_TIME;
		procentAnimacji *= procentAnimacji;// kwadratowa animacja - delte to sie liczy ze wzoru takiego co to nawet w liceum byl: delta = b^2 - 4ac
		procentAnimacji = powf(procentAnimacji, 0.8f);//wykladniczo tez jest fajnie, bo dodaje speeda na pocz¹tku
		float oneLessProcentAnimacji = 1 - procentAnimacji;

		// Animate UI
		_logo->setPosition(camPos + Vec2(0, visibleSize2.height * 0.4f + (visibleSize2.height * 0.8f) * oneLessProcentAnimacji));
		_info->setPosition(camPos + Vec2(0, visibleSize2.height * -1.2f + (visibleSize2.height * 0.2f) * procentAnimacji));
		float itemsOffset = 0;
		const float itemsMargin = 64 + 10;
		float itemsOffsetX = visibleSize2.width * 0.3f + (visibleSize2.width * 0.8f) * oneLessProcentAnimacji;
		_itemAmmo->setPosition(camPos + Vec2(itemsOffsetX, itemsOffset)); itemsOffset -= itemsMargin;
		_itemMed->setPosition(camPos + Vec2(itemsOffsetX, itemsOffset)); itemsOffset -= itemsMargin;
		_itemFreezer->setPosition(camPos + Vec2(itemsOffsetX, itemsOffset)); itemsOffset -= itemsMargin;

		// Check if can buy sth
		int cash = player->getCash();

		if (cash >= 2)
		{
			_itemAmmo->setTag(1);
			_itemAmmo->setColor(Color3B(255, 255, 255));
		}
		else
		{
			_itemAmmo->setTag(0);
			_itemAmmo->setColor(Color3B(255, 10, 10));
		}

		if (player->getHP() == 100)
		{
			_itemMed->setTag(0);
			_itemMed->setColor(Color3B(100, 100, 100));
		}
		else if (cash >= 5)
		{
			_itemMed->setTag(1);
			_itemMed->setColor(Color3B(255, 255, 255));
		}
		else
		{
			_itemMed->setTag(0);
			_itemMed->setColor(Color3B(255, 10, 10));
		}

		if (player->hasWeapon(Weapon::Type::Freezer))
		{
			_itemFreezer->setTag(0);
			_itemFreezer->setColor(Color3B(100, 100, 100));
		}
		else if (cash >= 10)
		{
			_itemFreezer->setTag(1);
			_itemFreezer->setColor(Color3B(255, 255, 255));
		}
		else
		{
			_itemFreezer->setTag(0);
			_itemFreezer->setColor(Color3B(255, 10, 10));
		}


		// Show UI
		_logo->setVisible(true);
		_info->setVisible(true);
		_itemAmmo->setVisible(true);
		_itemMed->setVisible(true);
		_itemFreezer->setVisible(true);

		AnyShopInUse |= _animTime >= 1;
	}
	else
	{
		// Hide UI
		_logo->setVisible(false);
		_info->setVisible(false);
		_itemAmmo->setVisible(false);
		_itemMed->setVisible(false);
		_itemFreezer->setVisible(false);
	}

	// Store current state
	_isOver = isOver;

	return false;
}

void Shop::onMouseDown(EventMouse* event)
{
	// Check if GUI is fully visible
	if (_animTime > 0)
	{

	}
}

void Shop::onMouseUp(EventMouse* event)
{
	// Check if GUI is fully visible
	if (_animTime > 0)
	{
		// Cache data
		Vec2 p;
		Size s;
		auto player = Game::getInstance()->getPlayer();
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Size visibleSize2 = visibleSize * 0.5f;
		auto camPos = _parent->getLevel()->getCamera()->getPosition();
		auto location = event->getLocationInView();
		location = Vec2(location.x, visibleSize.height + location.y);

		// Ammo
		p = _itemAmmo->getPosition() - camPos + visibleSize2;
		s = _itemAmmo->getContentSize();
		if (Rect(p.x, p.y - s.height / 2, s.width, s.height).containsPoint(location))
		{
			if (_itemAmmo->getTag() == 1)
			{
				// Buy
				player->addCash(-2);
				player->onPickupAmmo();
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/Trash_Sound.wav");
			}
			else
			{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/Nope_Sound.wav");
			}
		}

		// Med
		p = _itemMed->getPosition() - camPos + visibleSize2;
		s = _itemMed->getContentSize();
		if(Rect(p.x, p.y - s.height / 2, s.width, s.height).containsPoint(location))
		{
			if (_itemMed->getTag() == 1)
			{
				// Buy
				player->addCash(-5);
				player->onBuyMed();
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/Trash_Sound.wav");
			}
			else
			{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/Nope_Sound.wav");
			}
		}

		// Freezer
		p = _itemFreezer->getPosition() - camPos + visibleSize2;
		s = _itemFreezer->getContentSize();
		if(Rect(p.x, p.y - s.height / 2, s.width, s.height).containsPoint(location))
		{
			if (_itemFreezer->getTag() == 1)
			{
				// Buy
				player->addCash(-10);
				player->pickupWeapon(Weapon::Type::Freezer);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/Trash_Sound.wav");
			}
			else
			{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/Nope_Sound.wav");
			}
		}
	}
}
