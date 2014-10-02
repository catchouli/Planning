#include "goap/Agent.h"

namespace goap
{
	ActionWeakCollection Agent::makePlan(WorldState currentState,
		StateCollection desiredStates, const ActionWeakCollection& possibleActions)
	{
		// Build graph
		StateMap stateMap = buildStateMap(currentState, possibleActions);

		// Search graph
		std::vector<std::weak_ptr<Action>> plan = searchStateMap(stateMap, currentState, desiredStates);

		return plan;
	}

	StateMap Agent::buildStateMap(WorldState currentState,
		ActionWeakCollection possibleActions)
	{
		StateMap stateMap;

		// Recursively build map
		buildStateMapRecursive(stateMap, currentState, possibleActions);

		return stateMap;
	}

	void Agent::buildStateMapRecursive(StateMap& stateMap, WorldState currentState,
		ActionWeakCollection possibleActions)
	{
		// Get nodes linked to this state
		StateLinkSet& linkedStates = stateMap[currentState].linkedNodes;

		// Check possible actions and build links
		for (std::weak_ptr<Action> action : possibleActions)
		{
			if (action.lock()->isPossible(currentState))
			{
				WorldState resultState = action.lock()->updateState(currentState);

				if (resultState != currentState)
				{
					StateLink resultStateLink(resultState, action);

					if (linkedStates.find(resultStateLink) == linkedStates.end())
					{
						linkedStates.insert(resultStateLink);

						buildStateMapRecursive(stateMap, resultState, possibleActions);
					}
				}
			}
		}
	}

	ActionWeakCollection Agent::searchStateMap(const StateMap& stateMap, WorldState start,
		StateCollection desiredStates)
	{
		bool foundGoal = false;
		ActionWeakCollection plan;

		searchStateMapRecursive(stateMap, start, desiredStates,
			plan, std::unordered_set<WorldState>(), foundGoal);

		return plan;
	}

	void Agent::searchStateMapRecursive(const StateMap& stateMap, WorldState current,
		StateCollection desiredStates, ActionWeakCollection& actions,
		std::unordered_set<WorldState>& visitedStates, bool& foundGoal)
	{
		// Add this state to set so we don't revisit it
		visitedStates.insert(current);

		// Check if this is a goal state
		foundGoal = true;

		for (State state : desiredStates)
		{
			if (current.at(state.first) != state.second)
			{
				foundGoal = false;
				break;
			}
		}

		// Return if we've visited the goal state
		if (foundGoal)
			return;

		// Depth first search
		for (StateLink link : stateMap.at(current).linkedNodes)
		{
			// Push if we haven't visited this state on this path
			if (visitedStates.find(link.first) == visitedStates.end())
			{
				actions.push_back(link.second);
				searchStateMapRecursive(stateMap, link.first, desiredStates, actions, visitedStates, foundGoal);

				// Return if we've visited the goal state
				if (foundGoal)
					return;

				// Otherwise, pop the action and continue the search
				actions.pop_back();
			}
		}

		// Remove this state if we've finished searching it
		visitedStates.erase(current);
	}
}