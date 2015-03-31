#ifndef __MAIN_MENU_GUI_LAYER_H__
#define __MAIN_MENU_GUI_LAYER_H__

#include "cocos2d.h"
#include "MainMenuScene.h"

class MainMenuGUILayer : public cocos2d::Layer
{
public:

	virtual bool init();

	CREATE_FUNC(MainMenuGUILayer);

	// eventy
	void menuCloseCallback(cocos2d::Ref* pSender);

private:

	//MainMenuScene* _parent;
};

#endif // __MAIN_MENU_GUI_LAYER_H__