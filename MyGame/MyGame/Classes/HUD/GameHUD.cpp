
#include "GameHUD.h"
#include "DebugGUI.h"
#include "Game.h"

using namespace cocos2d;

bool GameHUD::init()
{
	if (!Layer::init())
	{
		return false;
	}

	scheduleUpdateWithPriority(100);

	const float marginUI = 10.0f;
	const char* fontUI = "Fonts/BACKTO1982.TTF";

	// pobierz dane o rozmiarze okna i srodku widoku
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ilosc pkt
	_score = Label::createWithTTF("Score: 10", fontUI, 30);
	_score->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_score->setPosition(Vec2(origin.x + marginUI, origin.y + visibleSize.height - marginUI));
	addChild(_score);

	// hp
	_hp = Label::createWithTTF("HP: 100", fontUI, 32);
	_hp->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	_hp->setPosition(Vec2(origin.x + visibleSize.width - marginUI, origin.y + visibleSize.height - marginUI));
	addChild(_hp);

	// ammo
	_ammo = Label::createWithTTF("Ammo 20 / 40", fontUI, 30);
	_ammo->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	_ammo->setPosition(Vec2(origin.x + visibleSize.width - marginUI, origin.y + marginUI));
	addChild(_ammo);

	_selectedGun = Sprite::create("Textures/mGun.png");
	_selectedGun->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_selectedGun->setPosition(10, -30);
	_selectedGun->setScale(2);
	wType = Weapon::Type::MachineGun;
	addChild(_selectedGun);

	// add debugging layer
	//auto debug = DebugGUI::create();
	//addChild(debug);

	return true;
}

void GameHUD::update(float dt)
{
	auto game = Game::getInstance();
	auto player = game->getPlayer();

	stringstream ss;

	ss << "Score: " << player->getScore() << endl << "Cash: " << player->getCash();
	_score->setString(ss.str());
	ss.str("");

	ss << "HP " << player->getHP();
	_hp->setString(ss.str());
	ss.str("");

	auto weapon = player->getGun();
	if (weapon->getType() == Weapon::Type::Pistol)
	{
		_ammo->setString("");
	}
	else
	{
		ss << "Ammo " << weapon->getMagazine() << " / " << weapon->getAmmo();
		_ammo->setString(ss.str());
		ss.str("");
	}

	if (wType != weapon->getType())
	{
		wType = weapon->getType();
		switch (wType)
		{
			case Weapon::Type::Pistol:
				_selectedGun->setTexture("Textures/pistol.png");
				break;
			case Weapon::Type::MachineGun: 
				_selectedGun->setTexture("Textures/mGun.png");
				break;
			case Weapon::Type::Freezer:
				_selectedGun->setTexture("Textures/freezer.png");
				break;
		}
	}
}
