#ifndef TESTPROJECT_PICKAXE_SMITH_H
#define TESTPROJECT_PICKAXE_SMITH_H

#include "Worker.h"

#include "../../actions/GetOre.h"
#include "../../actions/GetWood.h"
#include "../../actions/MakePick.h"
#include "../../actions/MakeAxe.h"
#include "../../actions/StorePick.h"
#include "../../actions/StoreAxe.h"

class PickaxeSmith
    : public Worker
{
public:
    PickaxeSmith(const glm::vec2& pos = glm::vec2());

    void update(float dt) override;

    goap::StateCollection getGoals() const override;

    goap::StateCollection getWorldState() const override;
};

PickaxeSmith::PickaxeSmith(const glm::vec2& pos)
    : Worker(pos, "Smith")
{
    // Add possible actions
    addAction<GetOre>();
    addAction<GetWood>();
    addAction<MakePick>();
    addAction<MakeAxe>();
    addAction<StorePick>();
    addAction<StoreAxe>();
}

void PickaxeSmith::update(float dt)
{
    // Do planning updates
    GoapEntity::update(dt);
}

goap::StateCollection PickaxeSmith::getGoals() const
{
    goap::StateCollection goals;

    goals[STATE_PICK_AVAILABLE] = true;

    return goals;
}

goap::StateCollection PickaxeSmith::getWorldState() const
{
    goap::StateCollection worldState;

    // Check if we have ore
    if (mInventory->getItemCount(ITEM_ORE) > 0)
    {
        worldState[STATE_HAS_ORE] = true;
    }

    // Check if we have a pickaxe
    if (mInventory->getItemCount(ITEM_PICKAXE) > 0)
    {
        worldState[STATE_HAS_PICK] = true;
    }

    // Check if we have an axe
    if (mInventory->getItemCount(ITEM_AXE) > 0)
    {
        worldState[STATE_HAS_AXE] = true;
    }

    // Check if there's ore / a pickaxe available in the first chest we find
    std::shared_ptr<Chest> chest = goap::entityCollection.findEntityByType<Chest>().lock();

    if (chest)
    {
        std::shared_ptr<Inventory> chestInv = chest->findComponentByType<Inventory>().lock();

        if (chestInv)
        {
            if (chestInv->getItemCount(ITEM_ORE) > 0)
            {
                worldState[STATE_ORE_AVAILABLE] = true;
            }

            if (chestInv->getItemCount(ITEM_WOOD) > 0)
            {
                worldState[STATE_WOOD_AVAILABLE] = true;
            }

            if (chestInv->getItemCount(ITEM_PICKAXE) > 0)
            {
                worldState[STATE_PICK_AVAILABLE] = true;
            }

            if (chestInv->getItemCount(ITEM_AXE) > 0)
            {
                worldState[STATE_AXE_AVAILABLE] = true;
            }
        }
    }

    return worldState;
}

#endif /* TESTPROJECT_PICKAXE_SMITH_H */