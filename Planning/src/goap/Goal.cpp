#include "goap/Goal.h"

namespace goap
{
	void Goal::addCondition(unsigned int state, bool value)
	{
		mConditions[state] = value;
	}

	bool Goal::satisfied(const WorldState& worldState)
	{
		bool satisfied = true;

		for (const auto& state : mConditions)
		{
			if (worldState.at(state.first) != state.second)
			{
				satisfied = false;
				break;
			}
		}

		return satisfied;
	}
}