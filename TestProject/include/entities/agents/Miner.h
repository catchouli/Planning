#ifndef TESTPROJECT_MINER_H
#define TESTPROJECT_MINER_H

#include "Worker.h"

#include "../../actions/GetPick.h"
#include "../../actions/MineOre.h"
#include "../../actions/StoreOre.h"

class Miner
	: public Worker
{
public:
    Miner(const glm::vec2& pos = glm::vec2());

	void update(float dt) override;

    goap::StateCollection getGoals() const override;

    goap::StateCollection getWorldState() const override;
};

Miner::Miner(const glm::vec2& pos)
    : Worker(pos, "Miner")
{
    // Add pickaxe
    mInventory->setItemCount(ITEM_PICKAXE, 1);

	// Add possible actions
    addAction<GetPick>();
    addAction<MineOre>();
	addAction<StoreOre>();
}

void Miner::update(float dt)
{
	// Do planning updates
	GoapEntity::update(dt);
}

goap::StateCollection Miner::getGoals() const
{
    goap::StateCollection goals;

    goals[STATE_ORE_AVAILABLE] = true;

    return goals;
}

goap::StateCollection Miner::getWorldState() const
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

    // Check if there's a pickaxe available in the first chest we find
    std::shared_ptr<Chest> chest = goap::entityCollection.findEntityByType<Chest>().lock();

    if (chest)
    {
        std::shared_ptr<Inventory> chestInv = chest->findComponentByType<Inventory>().lock();

        if (chestInv)
        {
            if (chestInv->getItemCount(ITEM_PICKAXE) > 0)
            {
                worldState[STATE_PICK_AVAILABLE] = true;
            }
        }
    }

    return worldState;
}

#endif /* TESTPROJECT_MINER_H */