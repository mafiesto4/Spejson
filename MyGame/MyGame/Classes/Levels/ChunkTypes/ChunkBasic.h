
#pragma once

#include <vector>
#include <cocos2d.h>
#include <string.h>
#include "../Chunk.h"

class ChunkBasic : public Chunk
{
public:

	ChunkBasic(Chunk* prevChunk)
		:Chunk(prevChunk)
	{
	}

	~ChunkBasic()
	{ }

	CHUNKS_CREATE(ChunkBasic);

private:

	cocos2d::Size getDesireSize() override;
	void generate() override;
};
