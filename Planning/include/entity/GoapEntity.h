#ifndef PLANNING_GOAP_AGENT_H
#define PLANNING_GOAP_AGENT_H

#include "Entity.h"
#include "../goap/Agent.h"

namespace goap
{
    enum class FSMState
    {
        FSM_STATE_IDLE,
        FSM_STATE_MOVE,
        FSM_STATE_ACT
    };

    class GoapEntity
		: public Agent
	{
	public:
        GoapEntity(const glm::vec2& pos = glm::vec2());

		virtual void update(float dt) override;
		virtual void render(void* renderer) override = 0;

    protected:
        void idleUpdate(float dt);
        void moveUpdate(float dt);
        void actUpdate(float dt);

        std::weak_ptr<Action> getCurrentAction();
        
    private:
        FSMState mFSMState = FSMState::FSM_STATE_IDLE;

        unsigned int mCurrentAction = 0;
        std::vector<std::weak_ptr<Action>> mCurrentPlan;

        WorldState convertWorldState(StateCollection collection);
    };

    inline std::weak_ptr<Action> GoapEntity::getCurrentAction()
    {
        if (mCurrentAction < mCurrentPlan.size())
            return mCurrentPlan[mCurrentAction];
        else
            return std::weak_ptr<Action>();
    }
}

#endif /* PLANNING_GOAP_AGENT_H */