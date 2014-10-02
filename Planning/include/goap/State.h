#ifndef PLANNING_STATE_H
#define PLANNING_STATE_H

#include <bitset>
#include <unordered_map>
#include <unordered_set>

namespace goap
{
	// The maximum number of state variables the world can use (default: 0 - 31)
	#define GOAP_WORLD_STATE_SIZE 32

	typedef std::pair<unsigned int, bool> State;
	typedef std::unordered_map<unsigned int, bool> StateCollection;
	typedef std::bitset<GOAP_WORLD_STATE_SIZE> WorldState;
}

#endif /* PLANNING_STATE_H */