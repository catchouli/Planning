#ifndef PLANNING_ACTION_H
#define PLANNING_ACTION_H

#include <entity/Entity.h>

#include "State.h"

namespace goap
{
	class Action
	{
	public:
		virtual bool checkProceduralPreconditions() = 0;
		virtual void activateAction() = 0;

		void addPrecondition(unsigned int state, bool value);
		void addEffect(unsigned int state, bool value);

		bool isPossible(WorldState currentState) const;
		WorldState updateState(WorldState currentState) const;

		std::weak_ptr<Entity> getTarget() const;

        void setActor(Entity* actor);
        Entity* getActor() const;

        bool needsTarget() const;

	protected:
		bool mNeedsTarget;
		std::weak_ptr<Entity> mTarget;
        
        Entity* mActor;

		StateCollection mPreconditions;
		StateCollection mEffects;
	};

	inline bool Action::isPossible(WorldState currentState) const
	{
		// Check if action is valid in this world state
		for (const auto& state : mPreconditions)
		{
			// Test state
			if (currentState.test(state.first) != state.second)
			{
				return false;
			}
		}

		return true;
	}

	inline WorldState Action::updateState(WorldState currentState) const
	{
		// Make sure we aren't trying this with an invalid state
		assert(isPossible(currentState));

		// Create a new world state after this action
		WorldState newState = currentState;

		// Copy resultant states to new world state
		for (const auto& state : mEffects)
		{
			newState[state.first] = state.second;
		}

		return newState;
	}

    inline bool Action::needsTarget() const
    {
        return mNeedsTarget;
    }

    inline std::weak_ptr<Entity> Action::getTarget() const
    {
        return mTarget;
    }

    inline void Action::setActor(Entity* actor)
    {
        mActor = actor;
    }

    inline Entity* Action::getActor() const
    {
        return mActor;
    }
}

#endif /* PLANNING_ACTION_H */