
#include "MainMenuGUILayer.h"
#include "ScoresScene.h"
#include <Levels/Level.h>
#include <Game.h>

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
	auto title = Label::createWithTTF("Spejson", "Fonts/BACKTO1982.ttf", 52);
	title->setColor(Color3B(255, 43, 106));
	title->setPosition(Vec2(x, origin.y + (visibleSize.height - title->getContentSize().height) * 0.8f));
	this->addChild(title, 1);

	// start gry
	auto play = MenuItemImage::create(
		"Textures/menuPlay.png",
		"Textures/menuPlay.png",
		CC_CALLBACK_1(MainMenuGUILayer::menuPlayCallback, this));

	const int offset = 20;
	int y = title->getPositionY() - title->getContentSize().height - offset - play->getContentSize().height / 2;

	play->setPosition(Vec2(x, y));
	y -= play->getContentSize().height + offset;

	// highscory
	auto opcje = MenuItemImage::create(
		"Textures/menuOptions.png",
		"Textures/menuOptions.png",
		CC_CALLBACK_1(MainMenuGUILayer::menuHighScoresCallback, this));
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

void MainMenuGUILayer::menuPlayCallback(Ref* pSender)
{
	Game::getInstance()->play();
}

void MainMenuGUILayer::menuHighScoresCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(ScoresScene::create());
}

void MainMenuGUILayer::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
}
