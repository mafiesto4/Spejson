
#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class MyCamera : public cocos2d::Camera
{
public:

	static MyCamera* Create()
	{
		MyCamera* camera = new (std::nothrow) MyCamera();
		camera->initDefault();
		camera->autorelease();

		return camera;
	}

	void ScaleThisShit()
	{
		//setAdditionalProjection
		

		Mat4 t;
		Mat4::createScale(10, 10, 10, &t);
		setAdditionalTransform(&t);



	}
};
