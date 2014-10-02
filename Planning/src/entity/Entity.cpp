#include "entity/Entity.h"

#include <glm/glm.hpp>

namespace goap
{
	Entity::Entity()
	{

	}

	Entity::Entity(const glm::vec2& position)
		: mPosition(position)
	{

	}

	void Entity::moveTowards(const glm::vec2& position, float dist)
	{
		// Get direction
		glm::vec2 diff = position - mPosition;
		float distToTarget = glm::length(diff);
		glm::vec2 dir = diff / distToTarget;

		if (distToTarget <= dist)
			mPosition = position;
		else
			mPosition += dir * dist;
	}
}