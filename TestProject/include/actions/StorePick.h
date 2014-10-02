#ifndef STORE_PICK_H
#define STORE_PICK_H

#include <entity/Entity.h>
#include <goap/Action.h>

#include "../data/WorldStates.h"
#include "../data/ItemIds.h"

#include "../entities/objects/Chest.h"

class StorePick
    : public goap::Action
{
public:
    StorePick();

    bool checkProceduralPreconditions() override;
    void activateAction() override;
};

StorePick::StorePick()
{
    mNeedsTarget = true;

    addPrecondition(STATE_HAS_PICK, true);

    addEffect(STATE_HAS_PICK, false);
    addEffect(STATE_PICK_AVAILABLE, true);
}

bool StorePick::checkProceduralPreconditions()
{
    // Find mine
    std::weak_ptr<const Chest> chest = goap::entityCollection.findEntityByType<Chest>();

    // Store target
    mTarget = chest;

    return true;
}

void StorePick::activateAction()
{
    // Remove pick from inventory
    // Get actor's inventory
    std::shared_ptr<Inventory> actorInv = mActor->findComponentByType<Inventory>().lock();

    // Remove pick
    actorInv->removeItem(ITEM_PICKAXE, 1);

    // Add pick to the chest
    // Get nearest chest's inventory
    std::shared_ptr<Chest> chest = std::dynamic_pointer_cast<Chest>(mTarget.lock());
    std::shared_ptr<Inventory> chestInv = chest->findComponentByType<Inventory>().lock();

    chestInv->addItem(ITEM_PICKAXE, 1);
}

#endif /* STORE_PICK_H */