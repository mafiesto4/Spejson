
#pragma once

#include <vector>
#include <cocos2d.h>
#include <string.h>
#include "../Chunk.h"

class BossFight : public Chunk
{
	friend class Boss;

private:

	cocos2d::Vec2 _pathPointForNext;

public:

	BossFight(Chunk* prevChunk)
		:Chunk(prevChunk)
	{
	}

	~BossFight()
	{
	}

	CHUNKS_CREATE(BossFight);

	cocos2d::Vec2 getPathPointForPrevChunk() const override;

private:

	cocos2d::Size getDesireSize() override;
	void calculatePathPoint() override;
	void generate() override;

	void onBossKilled();
};
