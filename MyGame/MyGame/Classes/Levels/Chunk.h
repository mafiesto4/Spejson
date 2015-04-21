
#pragma once

#include <vector>
#include <cocos2d.h>
#include <string.h>

// Describes single chunk with own logic and children
class Chunk : public cocos2d::Layer
{
public:

	// Chunk type
	enum Type
	{
		Basic = 0,
		ShoppingCentre,
		TwoWays
	};

private:

	int _seed;

public:

	// Gets chunk type
	inline virtual Type getType() = 0;
};
