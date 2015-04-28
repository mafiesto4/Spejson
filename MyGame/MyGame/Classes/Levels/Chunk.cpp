
#include <cocos2d.h>
#include <vector>
#include <string.h>
#include "physics\CCPhysicsWorld.h"


#include "Chunk.h"
#include "Level.h"
#include "ChunkTypes/ChunkBasic.h"

using namespace cocos2d;

Chunk::~Chunk()
{
	_child = nullptr;
}

bool Chunk::init()
{
	// generate seed
	_seed = rand();

	// Base
	//if (!Layer::init())
	if (!LayerColor::initWithColor(Color4B(rand() % 255, rand() % 255, rand() % 255, 255)))
	{
		return false;
	}

	return true;
}

void Chunk::Spread(Level* level)
{
	if (_child == nullptr)
	{
		// Add basic chunk at the top
		auto top = ChunkBasic::create();
		top->setPosition(getPosition() + Vec2(0, getContentSize().height));
		level->addChunk(top);
		_child = top;
	}
	else
	{
		_child->Spread(level);
	}
}

Sprite* Chunk::addPlatform(Vec2 location, float width)
{
	// Load texture and ensure its repeating
	Texture2D* texture = TextureCache::sharedTextureCache()->addImage("Textures/grass.png");
	Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	texture->setTexParameters(tp);

	// Create sprite
	auto sprite = Sprite::createWithTexture(texture, Rect(0, 0, width, CHUNKS_BLOCK_SIZE));
	sprite->setAnchorPoint(Vec2(0, 0));
	sprite->setPosition(location);
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

Sprite* Chunk::addWall(Vec2 location, float height)
{
	// Load texture and ensure its repeating
	Texture2D* texture = TextureCache::sharedTextureCache()->addImage("Textures/brick1.png");
	Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	texture->setTexParameters(tp);

	// Create sprite
	auto sprite = Sprite::createWithTexture(texture, Rect(0, 0, CHUNKS_BLOCK_SIZE, height));
	sprite->setAnchorPoint(Vec2(0, 0));
	sprite->setPosition(location);
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
