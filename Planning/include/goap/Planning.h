#ifndef GOAP_PLANNING_H
#define GOAP_PLANNING_H

#include "Types.h"
#include "Action.h"

#include <unordered_set>

typedef std::pair<WorldState, Action> StateLink;
typedef std::set<StateLink, StateLinkComparer> StateLinkSet;

struct StateLinkComparer
{
	bool operator() (const StateLink& a, const StateLink& b) const
	{
		return a.first.to_ulong() < b.first.to_ulong();
	}
};

struct StateNode
{
	// World states that are linked to this node and by which action
	StateLinkSet linkedNodes;
};

inline void buildStateMap(StateMap& stateMap, WorldState currentState, std::vector<Action> possibleActions)
{
	// Get nodes linked to this state
	StateLinkSet& linkedStates = stateMap[currentState].linkedNodes;

	// Check possible actions and build links
	for (const auto& action : possibleActions)
	{
		if (action.isPossible(currentState))
		{
			WorldState resultState = action.updateState(currentState);

			if (resultState != currentState)
			{
				StateLink resultStateLink(resultState, action);

				if (linkedStates.find(resultStateLink) == linkedStates.end())
				{
					linkedStates.insert(resultStateLink);

					buildStateMap(stateMap, resultState, possibleActions);
				}
			}
		}
	}
}

inline StateMap buildStateMap(WorldState currentState, std::vector<Action> possibleActions)
{
	StateMap stateMap;

	// Recursively build map
	buildStateMap(stateMap, currentState, possibleActions);

	return stateMap;
}

inline void searchStateMap(const StateMap& stateMap, WorldState current, StateCollection desiredStates, std::vector<Action>& actions, std::unordered_set<WorldState>& visitedStates, bool& foundGoal)
{
	// Add this state to set so we don't revisit it
	visitedStates.insert(current);

	// Check if this is a goal state
	foundGoal = true;

	for (const auto& state : desiredStates)
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
	for (const auto& link : stateMap.at(current).linkedNodes)
	{
		// Push if we haven't visited this state on this path
		if (visitedStates.find(link.first) == visitedStates.end())
		{
			actions.push_back(link.second);
			searchStateMap(stateMap, link.first, desiredStates, actions, visitedStates, foundGoal);

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

inline std::vector<Action> searchStateMap(const StateMap& stateMap, WorldState start, StateCollection desiredStates)
{
	bool foundGoal = false;
	std::vector<Action> plan;

	searchStateMap(stateMap, start, desiredStates, plan, std::unordered_set<WorldState>(), foundGoal);

	return plan;
}

inline std::vector<Action> makePlan(WorldState currentState, StateCollection desiredStates, std::vector<Action> possibleActions)
{
	// Build graph
	StateMap stateMap = buildStateMap(currentState, possibleActions);

	// Search graph
	std::vector<Action> plan = searchStateMap(stateMap, currentState, desiredStates);

	return plan;
}

#endif /* GOAP_PLANNING_H */