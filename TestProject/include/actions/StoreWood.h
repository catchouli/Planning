#ifndef STORE_WOOD_H
#define STORE_WOOD_H

#include <entity/Entity.h>
#include <goap/Action.h>

#include "../data/WorldStates.h"

#include "../entities/objects/Chest.h"

class StoreWood
    : public goap::Action
{
public:
    StoreWood();

    bool checkProceduralPreconditions() override;
    void activateAction() override;
};

StoreWood::StoreWood()
{
    mNeedsTarget = true;

    addPrecondition(STATE_HAS_WOOD, true);

    addEffect(STATE_HAS_WOOD, false);
    addEffect(STATE_WOOD_AVAILABLE, true);
}

bool StoreWood::checkProceduralPreconditions()
{
    // Find mine
    std::weak_ptr<const Chest> chest = goap::entityCollection.findEntityByType<Chest>();

    // Store target
    mTarget = chest;

    return true;
}

void StoreWood::activateAction()
{
    // Remove wood from inventory
    // Get actor's inventory
    std::shared_ptr<Inventory> actorInv = mActor->findComponentByType<Inventory>().lock();

    actorInv->removeItem(ITEM_WOOD, 1);

    // Add wood to the chest
    // Get nearest chest's inventory
    std::shared_ptr<Chest> chest = std::dynamic_pointer_cast<Chest>(mTarget.lock());
    std::shared_ptr<Inventory> chestInv = chest->findComponentByType<Inventory>().lock();

    chestInv->addItem(ITEM_WOOD, 1);
}

#endif /* STORE_WOOD_H */