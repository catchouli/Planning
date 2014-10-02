#ifndef PLANNING_STATEMAP_H
#define PLANNING_STATEMAP_H

#include "State.h"
#include "Action.h"

#include <set>
#include <unordered_map>

namespace goap
{
	struct StateNode;

	typedef std::unordered_map<WorldState, StateNode> StateMap;
	typedef std::pair<WorldState, std::weak_ptr<Action>> StateLink;

	struct StateLinkComparer
	{
		bool operator() (const StateLink& a, const StateLink& b) const
		{
			return a.first.to_ulong() < b.first.to_ulong();
		}
	};

	typedef std::set<StateLink, StateLinkComparer> StateLinkSet;

	struct StateNode
	{
		// World states that are linked to this node and by which action
		StateLinkSet linkedNodes;
	};
}

#endif /* PLANNING_STATEMAP_H */