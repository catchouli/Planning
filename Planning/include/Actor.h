#ifndef ACTOR_H
#define ACTOR_H

#include <vector>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <glm/vec2.hpp>

#include "goap/Action.h"

enum ActorFSMState
{
	ACTOR_FSM_IDLE,
	ACTOR_FSM_MOVE,
	ACTOR_FSM_ACT
};

class Actor
{
public:
	Actor(const glm::vec2& position, StateCollection& worldState);

	void update(float dt);
	void render(sf::RenderWindow& window);

	void addToGoal(unsigned int state, bool value);
	void addToState(unsigned int state, bool value);

	void addPossibleAction(Action action);

protected:
	void moveTowards(const glm::vec2& position, float dt);

	void idleUpdate(float dt);
	void moveUpdate(float dt);
	void actUpdate(float dt);

	WorldState makeState();

private:
	glm::vec2 mPosition;

	// Current plan and action
	Action mCurrentAction;
	std::vector<Action> mCurrentPlan;

	// Goal and state
	StateCollection mActorGoal;
	StateCollection mActorState;
	StateCollection& mWorldState;

	// Available actions
	std::vector<Action> mActorActions;

	ActorFSMState mFSMState;

	sf::RectangleShape mRectangle;
	sf::Font mFont;
};

#endif /* ACTOR_H */