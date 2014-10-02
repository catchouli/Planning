#ifndef TESTPROJECT_MINE_H
#define TESTPROJECT_MINE_H

#include <entity/Entity.h>

class Mine
	: public goap::Entity
{
public:
    Mine(const glm::vec2& pos = glm::vec2()) : goap::Entity(pos) {}

	void update(float dt) override {}
	void render(void* renderer) override {}
};

#endif /* TESTPROJECT_MINE_H */