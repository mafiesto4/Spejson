
#pragma once

#include <cocos2d.h>
#include <string.h>
#include "../Chunk.h"

class ChunkBasic : public Chunk
{
public:

	ChunkBasic(Chunk* prevChunk, Level* level)
		:Chunk(prevChunk, level)
	{
	}

	~ChunkBasic()
	{ }

	CHUNKS_CREATE(ChunkBasic);

protected:

	Size getDesireSize() override;
	void generate() override;
};
