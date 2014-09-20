#ifndef GOAP_ACTION_H
#define GOAP_ACTION_H

#include "Types.h"

#include <assert.h>

#include <glm/vec2.hpp>

struct Action
{
	Action(const glm::vec2& position)
		: position(position)
	{

	}

	Action(const glm::vec2& position, const char* name)
		: position(position), name(name)
	{

	}

	std::string name;

	StateCollection pre;
	StateCollection post;

	glm::vec2 position;

	void addPre(unsigned int state, bool value)
	{
		pre[state] = value;
	}

	void addPost(unsigned int state, bool value)
	{
		post[state] = value;
	}

	bool isPossible(const WorldState& currentState) const
	{
		// Check if action is valid in this world state
		for (const auto& state : pre)
		{
			// Test state
			if (currentState.test(state.first) != state.second)
			{
				return false;
			}
		}

		return true;
	}

	WorldState updateState(const WorldState& currentState) const
	{
		// Make sure we aren't trying this with an invalid state
		assert(isPossible(currentState));

		// Create a new world state after this action
		WorldState newState = currentState;

		// Copy resultant states to new world state
		for (const auto& state : post)
		{
			newState[state.first] = state.second;
		}

		return newState;
	}
};

#endif /* GOAP_ACTION_H */