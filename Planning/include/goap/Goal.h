#ifndef PLANNING_GOAL_H
#define PLANNING_GOAL_H

#include "State.h"

namespace goap
{
	class Goal
	{
	public:
		void addCondition(unsigned int state, bool value);
		bool satisfied(const WorldState& worldState);

	protected:
		StateCollection mConditions;
	};
}

#endif /* PLANNING_GOAL_H */