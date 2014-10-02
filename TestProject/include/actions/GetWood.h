#ifndef GET_WOOD_H
#define GET_WOOD_H

#include <entity/Entity.h>
#include <goap/Action.h>

#include "../data/WorldStates.h"
#include "../data/ItemIds.h"

#include "../entities/objects/Chest.h"
#include "../entities/objects/Inventory.h"

class GetWood
    : public goap::Action
{
public:
    GetWood();

    bool checkProceduralPreconditions() override;
    void activateAction() override;
};

GetWood::GetWood()
{
    mNeedsTarget = true;

    addPrecondition(STATE_HAS_WOOD, false);
    addPrecondition(STATE_WOOD_AVAILABLE, true);

    addEffect(STATE_HAS_WOOD, true);
    addEffect(STATE_WOOD_AVAILABLE, false);
}

bool GetWood::checkProceduralPreconditions()
{
    // Find chest
    std::shared_ptr<Chest> chest = goap::entityCollection.findEntityByType<Chest>().lock();
    std::shared_ptr<Inventory> chestInv = chest->findComponentByType<Inventory>().lock();

    if (chestInv->getItemCount(ITEM_WOOD) <= 0)
        return false;

    // Store target
    mTarget = chest;

    return true;
}

void GetWood::activateAction()
{
    // Remove wood from the chest
    // Get nearest chest's inventory
    std::shared_ptr<Chest> chest = std::dynamic_pointer_cast<Chest>(mTarget.lock());
    std::shared_ptr<Inventory> chestInv = chest->findComponentByType<Inventory>().lock();

    chestInv->removeItem(ITEM_WOOD, 1);

    // Add wood to inventory
    // Get actor's inventory
    std::shared_ptr<Inventory> actorInv = mActor->findComponentByType<Inventory>().lock();

    actorInv->addItem(ITEM_WOOD, 1);
}

#endif /* GET_WOOD_H */