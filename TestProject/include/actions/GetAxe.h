#ifndef GET_AXE_H
#define GET_AXE_H

#include <entity/Entity.h>
#include <goap/Action.h>

#include "../data/WorldStates.h"
#include "../data/ItemIds.h"

#include "../entities/objects/Chest.h"
#include "../entities/objects/Inventory.h"

class GetAxe
    : public goap::Action
{
public:
    GetAxe();

    bool checkProceduralPreconditions() override;
    void activateAction() override;
};

GetAxe::GetAxe()
{
    mNeedsTarget = true;

    addPrecondition(STATE_HAS_AXE, false);
    addPrecondition(STATE_AXE_AVAILABLE, true);

    addEffect(STATE_HAS_AXE, true);
    addEffect(STATE_AXE_AVAILABLE, false);
}

bool GetAxe::checkProceduralPreconditions()
{
    // Find chest
    std::shared_ptr<Chest> chest = goap::entityCollection.findEntityByType<Chest>().lock();
    std::shared_ptr<Inventory> chestInv = chest->findComponentByType<Inventory>().lock();

    if (chestInv->getItemCount(ITEM_AXE) <= 0)
        return false;

    // Store target
    mTarget = chest;

    return true;
}

void GetAxe::activateAction()
{
    // Add axe to inventory
    // Get actor's inventory
    std::shared_ptr<Inventory> actorInv = mActor->findComponentByType<Inventory>().lock();

    // Remove pick
    actorInv->addItem(ITEM_AXE, 1);

    // Remove axe from the chest
    // Get nearest chest's inventory
    std::shared_ptr<Chest> chest = std::dynamic_pointer_cast<Chest>(mTarget.lock());
    std::shared_ptr<Inventory> chestInv = chest->findComponentByType<Inventory>().lock();

    chestInv->removeItem(ITEM_AXE, 1);
}

#endif /* GET_AXE_H */