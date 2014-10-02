#include "entity/GoapEntity.h"

namespace goap
{
    const float SPEED = 100;

    GoapEntity::GoapEntity(const glm::vec2& pos)
        : Agent(pos)
    {

    }

	void GoapEntity::update(float dt)
	{
        switch (mFSMState)
        {
        case FSMState::FSM_STATE_IDLE:

            idleUpdate(dt);
            
            break;

        case FSMState::FSM_STATE_MOVE:

            moveUpdate(dt);

            break;

        case FSMState::FSM_STATE_ACT:

            actUpdate(dt);

            break;
        }
    }

    void GoapEntity::idleUpdate(float dt)
    {
        // Get current world state
        WorldState worldState = convertWorldState(getWorldState());

        // Build state map
        goap::StateMap stateMap = buildStateMap(worldState, getActions());

        // Find a plan
        mCurrentPlan = searchStateMap(stateMap, worldState, getGoals());

        // If the plan is valid
        if (mCurrentPlan.size() > 0)
        {
            // Reset action counter
            mCurrentAction = 0;

            // Start performing plan
            mFSMState = FSMState::FSM_STATE_MOVE;
        }
    }

    void GoapEntity::moveUpdate(float dt)
    {
        // Get current action
        std::shared_ptr<Action> currentAction = mCurrentPlan[mCurrentAction].lock();

        if (!currentAction->checkProceduralPreconditions())
        {
            // Invalidate current plan
            mCurrentPlan.clear();

            // We need a new plan
            mFSMState = FSMState::FSM_STATE_IDLE;
            return;
        }

        // Move towards location
        if (currentAction->needsTarget())
        {
            std::shared_ptr<Entity> target = currentAction->getTarget().lock();

            // Make sure target isn't null!!
            assert(target);

            // Get target location
            const glm::vec2& loc = currentAction->getTarget().lock()->getPosition();

            // Move towards target location
            moveTowards(loc, dt * SPEED);

            // Move onto doing action when we get there
            float dist = glm::length(loc - mPosition);
            if (dist < 32)
            {
                mFSMState = FSMState::FSM_STATE_ACT;
                return;
            }
        }
        else
        {
            // Move onto doing action immediately
            mFSMState = FSMState::FSM_STATE_ACT;
            return;
        }
    }

    void GoapEntity::actUpdate(float dt)
    {
        // Instantly perform action
        std::shared_ptr<Action> currentAction = mCurrentPlan[mCurrentAction].lock();
        currentAction->activateAction();

        // Move onto next action
        mCurrentAction++;

        // If the next action exists
        if (mCurrentAction < mCurrentPlan.size())
        {
            // Switch to next action
            mFSMState = FSMState::FSM_STATE_MOVE;
        }
        else
        {
            // We need a new plan
            mFSMState = FSMState::FSM_STATE_IDLE;
        }
    }

    WorldState GoapEntity::convertWorldState(StateCollection collection)
    {
        WorldState worldState;

        for (const auto& state : collection)
        {
            worldState[state.first] = state.second;
        }

        return worldState;
    }
}