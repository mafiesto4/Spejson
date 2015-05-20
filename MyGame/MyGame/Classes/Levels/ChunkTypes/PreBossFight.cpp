
#include <cocos2d.h>
#include <random>
#include "PreBossFight.h"
#include "../../Opponent/Alien1/Alien1.h"
#include "../../Opponent/Shooting/Shooting.h"
#include "BossFight.h"

using namespace cocos2d;

void PreBossFight::Spread(Level* level)
{
	if (_nextChunk == nullptr)
	{
		// Check if path point is in the middle
		Vec2 top = getPosition() + Vec2(0, getContentSize().height);
		if (_isValidPathPointLocation)
		{
			// Create boss fight chunk
			_nextChunk = BossFight::create(level, this, top);
		}
		else
		{
			// Create next pre boss fight until its path point is in the middle
			_nextChunk = PreBossFight::create(level, this, top);
		}

		// Generate itself chunk
		generate();
	}
	else
	{
		_nextChunk->Spread(level);
	}
}

void PreBossFight::calculatePathPoint()
{
	// Cache data
	auto size = getContentSize();
	int prevX = _previousChunk != nullptr ? _previousChunk->getPathPointForPrevChunk().x : 0;

	// Generate valid path point
	_pathPoint = getPosition() + AlignPointToGrid(Vec2(size.width * 0.5f, size.height * 0.5f));
	_isValidPathPointLocation = true;

	// Check if is valid
	while (abs(_pathPoint.x - prevX) <= 2)
	{
		// Generate invalid path point
		auto pos = Vec2(rand() % static_cast<int>(size.width * 0.6f), rand() % static_cast<int>(size.height * 0.5f));
		_pathPoint = getPosition() + AlignPointToGrid(pos + size * 0.1f);
		_isValidPathPointLocation = false;
	}
}
