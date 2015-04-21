
#pragma once

#include <vector>
#include <cocos2d.h>
#include <string.h>
#include "../Chunk.h"

class ChunkBasic : public Chunk
{
public:

	// Gets chunk type
	inline Type getType() override
	{
		return Type::Basic;
	}
};
