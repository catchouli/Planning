#ifndef PLANNING_AGENT_H
#define PLANNING_AGENT_H

#include <vector>
#include <memory>
#include <set>

#include "State.h"
#include "Action.h"
#include "StateMap.h"

#include "../entity/Entity.h"

namespace goap
{
	typedef std::vector<std::weak_ptr<Action>> ActionWeakCollection;
	typedef std::vector<std::shared_ptr<Action>> ActionStrongCollection;

	class Agent
        : public Entity
	{
	public:
        Agent(const glm::vec2& pos = glm::vec2());

		/* Add a type of action */
        template <typename T, typename... Args>
        void addAction(Args&&... args);

		/* Returns a collection of weak pointers to this agent's actions */
		ActionWeakCollection getActions() const;

        /* Allows the user to specify the agent's goals procedurally */
        virtual StateCollection getGoals() const = 0;

        /* Allows the user to specify the current world state procedurally */
        virtual StateCollection getWorldState() const = 0;

	protected:

		/* Called when the agent needs to generate a plan */
		ActionWeakCollection makePlan(WorldState currentState,
			StateCollection desiredStates, const ActionWeakCollection& possibleActions);

		/* Called when the agent needs to build a state map */
		StateMap buildStateMap(WorldState currentState,
			ActionWeakCollection possibleActions);

		/* Called when the agent needs to search a state map */
		ActionWeakCollection searchStateMap(const StateMap& stateMap, WorldState start,
			StateCollection desiredStates);

	private:

		/* The actions available to this agent */
		ActionStrongCollection mAvailableActions;

		/* The worker function for building a state map recursively */
		void buildStateMapRecursive(StateMap& stateMap, WorldState currentState,
			ActionWeakCollection possibleActions);

		/* The worker function for searching the state map recursively */
		void searchStateMapRecursive(const StateMap& stateMap, WorldState current,
			StateCollection desiredStates, ActionWeakCollection& actions,
			std::unordered_set<WorldState>& visitedStates, bool& foundGoal);
    };

    inline Agent::Agent(const glm::vec2& pos)
        : Entity(pos)
    {

    }

	template <typename T, typename... Args>
	void Agent::addAction(Args&&... args)
	{
        std::shared_ptr<Action> action = std::make_shared<T>(args...);

        action->setActor(this);

        mAvailableActions.push_back(action);
	}

	inline ActionWeakCollection Agent::getActions() const
	{
		ActionWeakCollection weakCollection;

		for (auto action : mAvailableActions)
			weakCollection.push_back(action);

		return weakCollection;
	}
}

#endif /* PLANNING_AGENT_H */