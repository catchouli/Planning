#ifndef MINE_ORE_H
#define MINE_ORE_H

#include <entity/Entity.h>
#include <goap/Action.h>

#include "../data/WorldStates.h"
#include "../data/ItemIds.h"

#include "../entities/objects/Mine.h"
#include "../entities/objects/Inventory.h"

class MineOre
	: public goap::Action
{
public:
	MineOre();

	bool checkProceduralPreconditions() override;
	void activateAction() override;
};

MineOre::MineOre()
{
	mNeedsTarget = true;

	addPrecondition(STATE_HAS_PICK, true);

	addEffect(STATE_HAS_ORE, true);
}

bool MineOre::checkProceduralPreconditions()
{
    // Get actor's inventory
    std::shared_ptr<Inventory> inv = mActor->findComponentByType<Inventory>().lock();

    // Fail if we don't have a pickaxe
    if (inv->getItemCount(ITEM_PICKAXE) == 0)
        return false;

    // Find mine
    std::weak_ptr<Mine> mine = goap::entityCollection.findEntityByType<Mine>();

    // Store target
    mTarget = mine;

    return true;
}

void MineOre::activateAction()
{
    // Add ore to inventory
    // Get actor's inventory
    std::shared_ptr<Inventory> actorInv = mActor->findComponentByType<Inventory>().lock();

    actorInv->addItem(ITEM_ORE, 1);

    // A 50% chance of breaking the actor's pickaxe
    float randf = rand() / (float)RAND_MAX;
    if (randf < 0.2f)
    {
        actorInv->removeItem(ITEM_PICKAXE, 1);
    }
}

#endif /* MINE_ORE_H */