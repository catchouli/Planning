#include "goap/Action.h"

namespace goap
{
	void Action::addPrecondition(unsigned int state, bool value)
	{
		mPreconditions[state] = value;
	}

	void Action::addEffect(unsigned int state, bool value)
	{
		mEffects[state] = value;
	}
}