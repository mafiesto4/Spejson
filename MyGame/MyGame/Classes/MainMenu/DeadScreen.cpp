
#include "DeadScreen.h"
#include "MainMenuScene.h"
#include <string>
#include "../Scores/Highscores.h"

USING_NS_CC;

using namespace std;

bool DeadScreen::init()
{
	// inicjalizacja sceny z cocos'a
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

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

	// back
	auto back = MenuItemImage::create(
		"Textures/menuBack.png",
		"Textures/menuBack.png",
		CC_CALLBACK_1(DeadScreen::backCallback, this));
	back->setPosition(200, 80);

	// create menu
	auto menu = Menu::create(back, NULL);
	menu->setPosition(Vec2::ZERO);
	addChild(menu, 1);

	// title
	Label* lTitle = Label::create("You are dead", "Fonts/BACKTO1982.ttf", 40);
	lTitle->setPosition(visibleSize.width / 2, visibleSize.height / 2 * 1.1f + 100);
	addChild(lTitle);

	// score
	stringstream score;
	score << "Score: " << _score;
	Label* lScore = Label::create(score.str(), "Fonts/BACKTO1982.ttf", 24);
	lScore->setPosition(visibleSize.width / 2, visibleSize.height / 2 * 1.1f);
	addChild(lScore);

	return true;
}

void DeadScreen::backCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(MainMenuScene::create());
}
