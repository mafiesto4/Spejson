
#include "MainMenuGUILayer.h"
#include "MainMenuScene.h"

USING_NS_CC;

bool MainMenuGUILayer::init()
{
	//_parent = &parent;

	// inicjalizacja warstwy z cocos'a
	if (!Layer::init())
	{
		return false;
	}

	// pobierz dane o rozmiarze okna i srodku widoku
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int x = origin.x + visibleSize.width / 2;

	// Tytul gry
	auto title = Label::createWithTTF("Nazwa Gry", "Fonts/Marker Felt.ttf", 52);
	title->setPosition(Vec2(x, origin.y + (visibleSize.height - title->getContentSize().height) * 0.8f));
	this->addChild(title, 1);

	// start gry
	auto play = MenuItemImage::create(
		"Textures/menuPlay.png",
		"Textures/menuPlay.png",
		CC_CALLBACK_1(MainMenuGUILayer::menuCloseCallback, this));

	const int offset = 20;
	int y = title->getPositionY() - title->getContentSize().height - offset - play->getContentSize().height / 2;

	play->setPosition(Vec2(x, y));
	y -= play->getContentSize().height + offset;

	// opcje
	auto opcje = MenuItemImage::create(
		"Textures/menuOptions.png",
		"Textures/menuOptions.png",
		CC_CALLBACK_1(MainMenuGUILayer::menuCloseCallback, this));
	opcje->setPosition(Vec2(x, y));
	y -= opcje->getContentSize().height + offset;

	// wyjscie
	auto wyjscie = MenuItemImage::create(
		"Textures/menuExit.png",
		"Textures/menuExit.png",
		CC_CALLBACK_1(MainMenuGUILayer::menuCloseCallback, this));
	wyjscie->setPosition(Vec2(x, y));
	y -= wyjscie->getContentSize().height + offset;

	// create menu
	auto menu = Menu::create(play, opcje, wyjscie, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

void MainMenuGUILayer::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
