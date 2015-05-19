
#pragma once

#include <vector>
#include <cocos2d.h>
#include <string.h>
#include "../Chunk.h"

class ShoppingCenter : public Chunk
{
private:

	cocos2d::Vec2 _pathPointForPrev;

public:

	ShoppingCenter(Chunk* prevChunk)
		:Chunk(prevChunk)
	{
	}

	~ShoppingCenter()
	{
	}

	CHUNKS_CREATE(ShoppingCenter);
	
	cocos2d::Vec2 getPathPointForPrevChunk() const override;

private:

	cocos2d::Size getDesireSize() override;
	void calculatePathPoint() override;
	void generate() override;
};
