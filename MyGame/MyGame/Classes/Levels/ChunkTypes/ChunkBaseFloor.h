
#pragma once

#include <vector>
#include <cocos2d.h>
#include <string.h>
#include "../Chunk.h"

class ChunkBaseFloor : public Chunk
{
public:

	~ChunkBaseFloor()
	{ }

	bool init() override;
	CHUNKS_CREATE(ChunkBaseFloor);

	inline Type getType() override
	{
		return Type::BaseFloor;
	}

	void Generate() override;
};
