#include "Actor.h"

#include "WorldStates.h"
#include "goap/Planning.h"

#include <glm/glm.hpp>
#include <SFML/Graphics/Text.hpp>

Actor::Actor(const glm::vec2& position, StateCollection& worldState)
	: mWorldState(worldState), mCurrentAction(glm::vec2(0, 0)),
	  mPosition(position), mFSMState(ACTOR_FSM_IDLE)
{
	mFont.loadFromFile("impact.ttf");
	mRectangle.setSize(sf::Vector2f(32.0f, 32.0f));
	mRectangle.setFillColor(sf::Color::Color(rand() % 255, rand() % 255, rand() % 255));
}

void Actor::update(float dt)
{
	switch (mFSMState)
	{
	case ACTOR_FSM_IDLE:
		idleUpdate(dt);
		break;
	case ACTOR_FSM_MOVE:
		moveUpdate(dt);
		break;
	case ACTOR_FSM_ACT:
		actUpdate(dt);
		break;
	}
}

void Actor::addToGoal(unsigned int state, bool value)
{
	mActorGoal[state] = value;
}

void Actor::addToState(unsigned int state, bool value)
{
	mActorState[state] = value;
}

void Actor::addPossibleAction(Action action)
{
	mActorActions.push_back(action);
}

void Actor::idleUpdate(float dt)
{
	if (mCurrentPlan.size() == 0)
	{
		printf("Calculating plan\n");

		// Otherwise, get new plan based on goal
		WorldState overallState = makeState();

		// Make plan
		mCurrentPlan = makePlan(overallState, mActorGoal, mActorActions);
	}

	if (mCurrentPlan.size() > 0)
	{
		// If we have a plan, perform the next step
		mCurrentAction = mCurrentPlan[0];
		mCurrentPlan.erase(mCurrentPlan.begin());

		mFSMState = ACTOR_FSM_MOVE;
		printf("Switching to action %s\n", mCurrentAction.name.c_str());
	}
}

void Actor::moveUpdate(float dt)
{
	// Move towards location of desired action
	moveTowards(mCurrentAction.position, dt);

	// Get distance
	glm::vec2 diff = mCurrentAction.position - mPosition;
	float dist = glm::length(diff);

	// If distance is low enough, end movement
	if (dist < 0.001f)
	{
		mFSMState = ACTOR_FSM_ACT;
	}
}

void Actor::actUpdate(float dt)
{
	// Perform action, this is going to happen immediately for demo purposes
	// As soon as the movement is complete, get next task
	mFSMState = ACTOR_FSM_IDLE;

	if (mCurrentAction.isPossible(makeState()))
	{
		// If current action is possible, complete it
		for (const auto& result : mCurrentAction.post)
		{
			mWorldState[result.first] = result.second;
		}
	}
	else
	{
		// Give up on plan
		mCurrentPlan.clear();
	}
}

WorldState Actor::makeState()
{
	// Combine actor and world state
	WorldState overallState;

	for (std::pair<unsigned int, bool> worldState : mWorldState)
	{
		overallState[worldState.first] = worldState.second;
	}

	for (std::pair<unsigned int, bool> actorState : mActorState)
	{
		overallState[actorState.first] = actorState.second;
	}

	return overallState;
}

void Actor::render(sf::RenderWindow& window)
{
	mRectangle.setPosition(mPosition.x, mPosition.y);
	window.draw(mRectangle);

	// Render text
	if (mFSMState != ACTOR_FSM_IDLE)
	{
		char buf[1024];
		sprintf_s(buf, "Current action: %s\n", mCurrentAction.name.c_str());

		sf::Text text;
		text.setFont(mFont);
		text.setCharacterSize(20);
		text.setColor(sf::Color::Green);
		text.setPosition(mPosition.x, mPosition.y + 32);
		text.setString(buf);

		window.draw(text);
	}
}

void Actor::moveTowards(const glm::vec2& position, float dt)
{
	const float speed = 100.0f;

	// Get direction
	glm::vec2 diff = position - mPosition;
	float dist = glm::length(diff);
	glm::vec2 dir = diff / dist;

	if (dist <= dt * speed)
		mPosition = position;
	else
		mPosition += dir * dt * speed;
}