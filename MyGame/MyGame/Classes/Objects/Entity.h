#pragma once

// Single game entity interface
class Entity
{
public:

	// Virtual destructor
	virtual ~Entity() { }

	// Update entity
	virtual void update(class Chunk* parent, float dt) { }

	// Get main node
	virtual cocos2d::Sprite* getNode() const = 0;
};
