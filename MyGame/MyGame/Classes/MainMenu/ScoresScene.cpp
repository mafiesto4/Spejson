
#include "ScoresScene.h"
#include "MainMenuScene.h"
#include <string>
#include "../Scores/Highscores.h"

USING_NS_CC;

using namespace std;

bool ScoresScene::init()
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
		CC_CALLBACK_1(ScoresScene::backCallback, this));

	back->setPosition(200, 80);

	// create menu
	auto menu = Menu::create(back, NULL);
	menu->setPosition(Vec2::ZERO);
	addChild(menu, 1);

	// high scores
	stringstream scores;
	if (Scores._table.Count() > 0)
	{
		Scores._table.Sort();
		for (int i = Scores._table.Count() - 1; i >= 0; i--)
		{
			auto record = Scores._table[i];
			scores << record.Name << " - " << record.Score << endl;
		}
	}
	else
	{
		scores << "No scores";
	}
	Label* lScores = Label::create(scores.str(), "Fonts/BACKTO1982.ttf", 24);
	lScores->setPosition(visibleSize.width / 2, visibleSize.height / 2 * 1.1f);
	addChild(lScores);

	return true;
}

void ScoresScene::backCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(MainMenuScene::create());
}
