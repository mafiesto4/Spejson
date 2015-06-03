
#include "BloodHit.h"

BloodHit::BloodHit(Color3B color, float scale, float alpha)
	:_color(color),
	_scale(scale),
	_alpha(alpha)
{
	
}

BloodHit* BloodHit::createWithTotalParticles(int numberOfParticles, Color3B color, float scale, float alpha)
{
	BloodHit* ret = new (std::nothrow) BloodHit(color, scale, alpha);
	if (ret && ret->initWithTotalParticles(numberOfParticles))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool BloodHit::initWithTotalParticles(int numberOfParticles)
{
	if (ParticleExplosion::initWithTotalParticles(numberOfParticles))
	{
		// duration
		_duration = 0.1f;

		// Emitter mode: Gravity Mode
		setEmitterMode(Mode::GRAVITY);

		// Gravity Mode: gravity
		setGravity(Vec2(0, -400));

		// size, in pixels
		_startSize = 8.0f * _scale;
		_startSizeVar = 10.0f;
		_endSize = START_SIZE_EQUAL_TO_END_SIZE;

		// life of particles
		_life = 1.1f;
		_lifeVar = 1;

		// angle
		_angle = 69;
		_angleVar = 5;

		// Gravity Mode: radial acceleration
		setRadialAccel(0.1f);
		setRadialAccelVar(0);
		
		/*
		// Gravity Mode: speed of particles
		setSpeed(25);
		setSpeedVar(10);

		// emitter position
		Size winSize = Director::getInstance()->getWinSize();
		this->setPosition(winSize.width / 2, 0);
		setPosVar(Vec2(20, 0));

		// emits per frame
		_emissionRate = _totalParticles / _life;*/

		// color of particles
		_startColor.r = _color.r / 255.0f;
		_startColor.g = _color.g / 255.0f;
		_startColor.b = _color.b / 255.0f;
		_startColor.a = 0.9f * _alpha;
		_startColorVar.r = 0.02f;
		_startColorVar.g = 0.02f;
		_startColorVar.b = 0.02f;
		_startColorVar.a = 0.0f;
		_endColor.r = (10 + _color.r) / 300.0f;
		_endColor.g = (10 + _color.g) / 300.0f;
		_endColor.b = (10 + _color.b) / 300.0f;
		_endColor.a = 0.6f * _alpha;
		_endColorVar.r = 0.0f;
		_endColorVar.g = 0.0f;
		_endColorVar.b = 0.0f;
		_endColorVar.a = 0.0f;


		if (_texture)
		{
			Texture2D::TexParams tp = { GL_POINT, GL_POINT, GL_REPEAT, GL_REPEAT };
			_texture->setTexParameters(tp);
		}

		return true;
	}
	return false;
}
