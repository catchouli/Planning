#ifndef TESTPROJECT_LUMBERJACK_H
#define TESTPROJECT_LUMBERJACK_H

#include "Worker.h"

#include "../../actions/GetAxe.h"
#include "../../actions/StoreWood.h"
#include "../../actions/ChopWood.h"

#include "../../data/ItemIds.h"

class Lumberjack
    : public Worker
{
public:
    Lumberjack(const glm::vec2& pos = glm::vec2());

    void update(float dt) override;

    goap::StateCollection getGoals() const override;

    goap::StateCollection getWorldState() const override;
};

Lumberjack::Lumberjack(const glm::vec2& pos)
    : Worker(pos, "Lumberjack")
{
    // Add axe to inventory
    mInventory->setItemCount(ITEM_AXE, 1);

    // Add possible actions
    addAction<GetAxe>();
    addAction<StoreWood>();
    addAction<ChopWood>();
}

void Lumberjack::update(float dt)
{
    // Do planning updates
    GoapEntity::update(dt);
}

goap::StateCollection Lumberjack::getGoals() const
{
    goap::StateCollection goals;

    goals[STATE_WOOD_AVAILABLE] = true;

    return goals;
}

goap::StateCollection Lumberjack::getWorldState() const
{
    goap::StateCollection worldState;

    // Check if we have wood
    if (mInventory->getItemCount(ITEM_WOOD) > 0)
    {
        worldState[STATE_HAS_WOOD] = true;
    }

    // Check if we have an axe
    if (mInventory->getItemCount(ITEM_AXE) > 0)
    {
        worldState[STATE_HAS_AXE] = true;
    }

    // Check if there's an axe available in the first chest we find
    std::shared_ptr<Chest> chest = goap::entityCollection.findEntityByType<Chest>().lock();

    if (chest)
    {
        std::shared_ptr<Inventory> chestInv = chest->findComponentByType<Inventory>().lock();

        if (chestInv)
        {
            if (chestInv->getItemCount(ITEM_AXE) > 0)
            {
                worldState[STATE_AXE_AVAILABLE] = true;
            }
        }
    }

    return worldState;
}

#endif /* TESTPROJECT_LUMBERJACK_H */