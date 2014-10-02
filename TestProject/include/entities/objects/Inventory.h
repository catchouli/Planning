#ifndef TESTPROJECT_INVENTORY_H
#define TESTPROJECT_INVENTORY_H

#include <entity/Entity.h>

#include <unordered_map>

typedef std::unordered_map<unsigned int, unsigned int> ItemMap;

class Inventory
    : public goap::Entity
{
public:
    Inventory() {}

    void update(float dt) override {}
    void render(void* renderer) override {}

    void removeItem(unsigned int id, unsigned int count);
    void addItem(unsigned int id, unsigned int count);

    void setItemCount(unsigned int id, unsigned int count);
    unsigned int getItemCount(unsigned int id);

    const ItemMap& getItems() const;

private:
    ItemMap mItemCounts;
};

void Inventory::addItem(unsigned int id, unsigned int count)
{
    //assert(count > 0);

    mItemCounts[id] += count;
}

void Inventory::removeItem(unsigned int id, unsigned int count)
{
    //assert(mItemCounts[id] >= count);

    mItemCounts[id] -= count;
}

void Inventory::setItemCount(unsigned int id, unsigned int count)
{
    mItemCounts[id] = count;
}

unsigned int Inventory::getItemCount(unsigned int id)
{
    // Check for item in inventory and return 0 if it's missing
    if (mItemCounts.find(id) != mItemCounts.end())
        return mItemCounts.at(id);
    else
        return 0;
}

const ItemMap& Inventory::getItems() const
{
    return mItemCounts;
}

#endif /* TESTPROJECT_INVENTORY_H */