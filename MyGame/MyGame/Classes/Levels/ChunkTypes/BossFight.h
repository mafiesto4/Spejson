
#pragma once

#include <cocos2d.h>
#include <string.h>
#include "../Chunk.h"

class BossFight : public Chunk
{
	friend class Boss;

private:

	Vec2 _pathPointForNext;
	Sprite* _blockingPlatform;

public:

	BossFight(Chunk* prevChunk, Level* level)
		:Chunk(prevChunk, level)
	{
	}

	~BossFight()
	{
	}

	CHUNKS_CREATE(BossFight);

	Vec2 getPathPointForPrevChunk() const override;

private:

	Size getDesireSize() override;
	void calculatePathPoint() override;
	void generate() override;

	void onBossKilled();
};
