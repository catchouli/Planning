#ifndef TESTPROJECT_SMELTERY_H
#define TESTPROJECT_SMELTERY_H

#include <entity/Entity.h>

class Smeltery
	: public goap::Entity
{
public:
    Smeltery(const glm::vec2& pos = glm::vec2()) : goap::Entity(pos) {}

	void update(float dt) override {}
	void render(void* renderer) override {}
};

#endif /* TESTPROJECT_SMELTERY_H */