
#include <cocos2d.h>
#include <vector>
#include <string.h>
#include "physics\CCPhysicsWorld.h"


#include "Chunk.h"
#include "Level.h"
#include "ChunkTypes/ChunkBasic.h"
#include "../Objects/Ladder.h"

using namespace cocos2d;

Chunk::~Chunk()
{

}

bool Chunk::init()
{
	// Base
	//if (!Layer::init())
	if (!LayerColor::initWithColor(Color4B(rand() % 255, rand() % 255, rand() % 255, 255)))
	{
		return false;
	}

	// Set size
	setContentSize(getDesireSize());

	// Calculate path point
	calculatePathPoint();

	// Good
	return true;
}
void Chunk::calculatePathPoint()
{
	auto size = getContentSize();
	int prevX = _previousChunk != nullptr ? _previousChunk->getPathPoint().x : 0;
	do
	{
		auto pos = Vec2(rand() % static_cast<int>(size.width * 0.6f), rand() % static_cast<int>(size.height * 0.5f));
		_pathPoint = getPosition() + AlignPointToGrid(pos + size * 0.1f);

		//const int LOCAL_I = 0;
		//_pathPoint = getPosition() + Vec2(LOCAL_I % (int)size.width, 64);
		//LOCAL_I += CHUNKS_BLOCK_SIZE;

	} while (abs(_pathPoint.x - prevX) <= 2);
}

void Chunk::Spread(Level* level)
{
	if (_nextChunk == nullptr)
	{
		// Add basic chunk at the top
		_nextChunk = ChunkBasic::create(level, this, getPosition() + Vec2(0, getContentSize().height));

		// Generate itself chunk
		generate();
	}
	else
	{
		_nextChunk->Spread(level);
	}
}

Sprite* Chunk::addPlatform(Vec2 location, float width)
{
	// Load texture and ensure its repeating
	Texture2D* texture = TextureCache::sharedTextureCache()->addImage("Textures/brick1.png");
	Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	texture->setTexParameters(tp);

	// Create sprite
	auto sprite = Sprite::createWithTexture(texture, Rect(0, 0, width, CHUNKS_BLOCK_SIZE));
	sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	//sprite->setPosition(location);
	sprite->setPosition(Vec2(location.x + width / 2, location.y));
	//sprite->setTag(PHYSICS_TAG_GROUND);

	// Create body
	auto body = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.52f));
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	sprite->setPhysicsBody(body);

	// Add and back to the future
	addChild(sprite);
	return sprite;
}

Sprite* Chunk::addLadder(Vec2 location, float height)
{

	/*// Load texture and ensure its repeating
	Texture2D* texture = TextureCache::sharedTextureCache()->addImage("Textures/ladder.png");
	Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	texture->setTexParameters(tp);

	// Create sprite
	auto sprite = Sprite::createWithTexture(texture, Rect(0, 0, CHUNKS_BLOCK_SIZE, height));
	sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	sprite->setPosition(location);
	*/
	auto sprite = Ladder::Create(location, height);

	// Add and back to the future
	addChild(sprite);
	return sprite;
}

Sprite* Chunk::addWall(char dir)
{
	// Load texture and ensure its repeating
	Texture2D* texture = TextureCache::sharedTextureCache()->addImage("Textures/dirt.png");
	Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	texture->setTexParameters(tp);

	// Calculate size and location
	Vec2 size, position;
	auto chunkSize = getContentSize();
	switch (dir)
	{
		// super dluga podloga dla basefloorchunk
		case '_':
		{
			size = Vec2(chunkSize.width * 3, CHUNKS_DEFAULT_HEIGHT);
			position = Vec2(chunkSize.width / 2, -size.y / 2);
		}
		break;

		// podloga
		case 's':
		{
			size = Vec2(chunkSize.width, CHUNKS_DEFAULT_HEIGHT);
			position = Vec2(size.x / 2, -size.y / 2);
		}
		break;

		// sciana na lewo
		case 'a':
		{
			size = Vec2(CHUNKS_WIDTH, chunkSize.height);
			position = Vec2(-size.x / 2, size.y / 2);
		}
		break;

		// sciana na prawo
		case 'd':
		{
			size = Vec2(CHUNKS_WIDTH, chunkSize.height);
			position = Vec2(size.x / 2 * 3, size.y / 2);
		}
		break;

		// error
		default: assert(0); break;
	}

	// Create sprite
	auto sprite = Sprite::createWithTexture(texture, Rect(0, 0, size.x, size.y));
	sprite->setPosition(position);
	//sprite->setTag(PHYSICS_TAG_GROUND);

	// Create body
	auto body = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.52f));
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	sprite->setPhysicsBody(body);

	// Add and back to the future
	addChild(sprite);
	return sprite;
}
