#ifndef GOAP_TYPES_H
#define GOAP_TYPES_H

#include <bitset>
#include <unordered_map>
#include <set>
#include <vector>
#include <stack>

// Types
struct Action;
struct StateNode;
struct StateLinkComparer;

typedef std::pair<unsigned int, bool> State;
typedef std::unordered_map<unsigned int, bool> StateCollection;
typedef std::bitset<512> WorldState;
typedef std::unordered_map<WorldState, StateNode> StateMap;

#endif /* GOAP_TYPES_H */