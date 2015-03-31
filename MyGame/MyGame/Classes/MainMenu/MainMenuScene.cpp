
#include "MainMenuScene.h"
#include "..\Player\Player.h"

USING_NS_CC;

bool MainMenuScene::init()
{
	// inicjalizacja sceny z cocos'a
	if (!Scene::init())
	{
		return false;
	}

	// stworz warstwe z GUI
	auto _gui = MainMenuGUILayer::create();
	addChild(_gui);

	auto Gracz = new Player("xd");
	Gracz->add(this);

	return true;
}

