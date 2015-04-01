
#include "GameHUD.h"
using namespace cocos2d;

bool GameHUD::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// pobierz dane o rozmiarze okna i srodku widoku
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// nazwa gracza
	auto playerName = Label::createWithTTF("Player1", "Fonts/Marker Felt.ttf", 24);
	auto playerNameSize = playerName->getContentSize();
	playerName->setAnchorPoint(Vec2::ZERO);
	playerName->setPosition(Vec2(origin.x + 10, origin.y + visibleSize.height - 10 - playerNameSize.height));
	this->addChild(playerName, 1);

	return true;
}