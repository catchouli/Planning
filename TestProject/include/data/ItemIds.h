#ifndef TESTPROJECT_ITEM_IDS_H
#define TESTPROJECT_ITEM_IDS_H

#include <array>

// Item Ids
enum ItemIds
{
    ITEM_PICKAXE,
    ITEM_AXE,
    ITEM_ORE,
    ITEM_WOOD
};

std::array<const char*, 4> ItemNames =
{
    "Pickaxe",
    "Axe",
    "Ore",
    "Wood"
};

#endif /* TESTPROJECT_ITEM_IDS_H */