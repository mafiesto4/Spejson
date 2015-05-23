
#include "GameHUD.h"
#include "DebugGUI.h"
using namespace cocos2d;

bool GameHUD::init()
{
	if (!Layer::init())
	{
		return false;
	}

	scheduleUpdateWithPriority(100);

	// pobierz dane o rozmiarze okna i srodku widoku
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ilosc pkt
	_score = Label::createWithTTF("Score", "Fonts/Marker Felt.ttf", 24);
	auto scoreLabelSize = _score->getContentSize();
	_score->setAnchorPoint(Vec2::ZERO);
	_score->setPosition(Vec2(origin.x + 10, origin.y + visibleSize.height - 10 - scoreLabelSize.height));
	addChild(_score);

	// hp
	_hp = Label::createWithTTF("100", "Fonts/Marker Felt.ttf", 24);
	auto hpLabelSize = _hp->getContentSize();
	_hp->setAnchorPoint(Vec2::ZERO);
	_hp->setPosition(Vec2(origin.x + visibleSize.width - 10 - hpLabelSize.width, origin.y + visibleSize.height - 10 - hpLabelSize.height));
	addChild(_hp);

	// ammo
	_ammo = Label::createWithTTF("Ammo x 30", "Fonts/Marker Felt.ttf", 24);
	auto ammoLabelSize = _ammo->getContentSize();
	_ammo->setAnchorPoint(Vec2::ZERO);
	_ammo->setPosition(Vec2(origin.x + visibleSize.width - 120 - hpLabelSize.width, origin.y + 10));
	addChild(_ammo);

	// liczba ¿yæ
	_lifes = Label::createWithTTF("Lifes x 3", "Fonts/Marker Felt.ttf", 24);
	auto lifesLabelSize = _lifes->getContentSize();
	_lifes->setAnchorPoint(Vec2::ZERO);
	_lifes->setPosition(Vec2(origin.x + 10, origin.y + 10));
	addChild(_lifes);

	// add debugging layer
	//auto debug = DebugGUI::create();
	//addChild(debug);

	return true;
}

void GameHUD::update(float dt)
{

}
