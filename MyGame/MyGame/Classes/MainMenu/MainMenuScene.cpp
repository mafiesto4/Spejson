
#include "MainMenuScene.h"
#include "MainMenuGUILayer.h"

USING_NS_CC;

bool MainMenuScene::init()
{
	// inicjalizacja sceny z cocos'a
	if (!Scene::init())
	{
		return false;
	}
	
	auto visibleSize = Director::getInstance()->getVisibleSize();

	// stworz warstwe z GUI
	auto _gui = MainMenuGUILayer::create();
	addChild(_gui);

	// background
	Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("Textures/bg.png");
	if (texture == nullptr)
	{
		return false;
	}
	auto texSize = texture->getContentSize();
	Rect rect = Rect::ZERO;
	rect.size = texSize;
	auto background = Sprite::createWithTexture(texture, rect, false);
	background->setScaleX(visibleSize.width / texSize.width);
	background->setScaleY(visibleSize.height / texSize.height);
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	addChild(background, -1000000);

	return true;
}
