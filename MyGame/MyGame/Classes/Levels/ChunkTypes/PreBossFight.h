
#pragma once

#include <cocos2d.h>
#include <string.h>
#include "../Chunk.h"
#include "ChunkBasic.h"

class PreBossFight : public ChunkBasic
{
	friend class Boss;

private:

	bool _isValidPathPointLocation;
	cocos2d::Vec2 _pathPointForNext;

public:

	PreBossFight(Chunk* prevChunk, Level* level)
		:ChunkBasic(prevChunk, level)
	{
	}

	~PreBossFight()
	{
	}

	CHUNKS_CREATE(PreBossFight);
	
	void Spread(Level* level) override;

private:

	void calculatePathPoint() override;
};
