
#pragma once

#include <vector>
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

	PreBossFight(Chunk* prevChunk)
		:ChunkBasic(prevChunk)
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
