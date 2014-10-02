#ifndef STORE_AXE_H
#define STORE_AXE_H

#include <entity/Entity.h>
#include <goap/Action.h>

#include "../data/WorldStates.h"
#include "../data/ItemIds.h"

#include "../entities/objects/Chest.h"

class StoreAxe
    : public goap::Action
{
public:
    StoreAxe();

    bool checkProceduralPreconditions() override;
    void activateAction() override;
};

StoreAxe::StoreAxe()
{
    mNeedsTarget = true;

    addPrecondition(STATE_HAS_AXE, true);

    addEffect(STATE_HAS_AXE, false);
    addEffect(STATE_AXE_AVAILABLE, true);
}

bool StoreAxe::checkProceduralPreconditions()
{
    // Find mine
    std::weak_ptr<const Chest> chest = goap::entityCollection.findEntityByType<Chest>();

    // Store target
    mTarget = chest;

    return true;
}

void StoreAxe::activateAction()
{
    // Remove axe from inventory
    // Get actor's inventory
    std::shared_ptr<Inventory> actorInv = mActor->findComponentByType<Inventory>().lock();

    // Remove axe
    actorInv->removeItem(ITEM_AXE, 1);

    // Add axe to the chest
    // Get nearest chest's inventory
    std::shared_ptr<Chest> chest = std::dynamic_pointer_cast<Chest>(mTarget.lock());
    std::shared_ptr<Inventory> chestInv = chest->findComponentByType<Inventory>().lock();

    chestInv->addItem(ITEM_AXE, 1);
}

#endif /* STORE_AXE_H */