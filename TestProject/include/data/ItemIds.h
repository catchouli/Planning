#ifndef TESTPROJECT_ITEM_IDS_H
#define TESTPROJECT_ITEM_IDS_H

#include <array>

// Item Ids
enum ItemIds
{
    ITEM_PICKAXE,
    ITEM_ORE
};

std::array<const char*, 2> ItemNames =
{
    "Pickaxe",
    "Ore"
};

#endif /* TESTPROJECT_ITEM_IDS_H */