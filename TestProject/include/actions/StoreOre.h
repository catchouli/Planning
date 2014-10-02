#ifndef STORE_ORE_H
#define STORE_ORE_H

#include <entity/Entity.h>
#include <goap/Action.h>

#include "../data/WorldStates.h"

#include "../entities/objects/Chest.h"

class StoreOre
	: public goap::Action
{
public:
	StoreOre();

	bool checkProceduralPreconditions() override;
	void activateAction() override;
};

StoreOre::StoreOre()
{
	mNeedsTarget = true;

	addPrecondition(STATE_HAS_ORE, true);

    addEffect(STATE_HAS_ORE, false);
    addEffect(STATE_ORE_AVAILABLE, true);
}

bool StoreOre::checkProceduralPreconditions()
{
    // Find mine
    std::weak_ptr<const Chest> chest = goap::entityCollection.findEntityByType<Chest>();

    // Store target
    mTarget = chest;

    return true;
}

void StoreOre::activateAction()
{
    // Remove ore from inventory
    // Get actor's inventory
    std::shared_ptr<Inventory> actorInv = mActor->findComponentByType<Inventory>().lock();

    actorInv->removeItem(ITEM_ORE, 1);

    // Add ore to the chest
    // Get nearest chest's inventory
    std::shared_ptr<Chest> chest = std::dynamic_pointer_cast<Chest>(mTarget.lock());
    std::shared_ptr<Inventory> chestInv = chest->findComponentByType<Inventory>().lock();

    chestInv->addItem(ITEM_ORE, 1);
}

#endif /* STORE_ORE_H */