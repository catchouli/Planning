#ifndef GET_ORE_H
#define GET_ORE_H

#include <entity/Entity.h>
#include <goap/Action.h>

#include "../data/WorldStates.h"
#include "../data/ItemIds.h"

#include "../entities/objects/Chest.h"

class GetOre
	: public goap::Action
{
public:
	GetOre();

	bool checkProceduralPreconditions() override;
	void activateAction() override;
};

GetOre::GetOre()
{
	mNeedsTarget = true;

    addPrecondition(STATE_HAS_ORE, false);
    addPrecondition(STATE_ORE_AVAILABLE, true);

    addEffect(STATE_HAS_ORE, true);
    addEffect(STATE_ORE_AVAILABLE, false);
}

bool GetOre::checkProceduralPreconditions()
{
    // Find mine
    std::weak_ptr<const Chest> chest = goap::entityCollection.findEntityByType<Chest>();

    // Store target
    mTarget = chest;

	return true;
}

void GetOre::activateAction()
{
    // Remove ore from the chest
    // Get nearest chest's inventory
    std::shared_ptr<Chest> chest = std::dynamic_pointer_cast<Chest>(mTarget.lock());
    std::shared_ptr<Inventory> chestInv = chest->findComponentByType<Inventory>().lock();

    chestInv->removeItem(ITEM_ORE, 1);

    // Add ore to inventory
    // Get actor's inventory
    std::shared_ptr<Inventory> actorInv = mActor->findComponentByType<Inventory>().lock();

    actorInv->addItem(ITEM_ORE, 1);
}

#endif /* GET_ORE_H */