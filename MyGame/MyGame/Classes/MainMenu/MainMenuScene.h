#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"
#include "MainMenuGUILayer.h"

class MainMenuScene : public cocos2d::Scene
{
public:

	virtual bool init();

	CREATE_FUNC(MainMenuScene);

private:

	//MainMenuGUILayer* _gui;
};

#endif // __MAIN_MENU_SCENE_H__
