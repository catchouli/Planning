#ifndef GET_PICK_H
#define GET_PICK_H

#include <entity/Entity.h>
#include <goap/Action.h>

#include "../data/WorldStates.h"
#include "../data/ItemIds.h"

#include "../entities/objects/Chest.h"

class GetPick
    : public goap::Action
{
public:
    GetPick();

    bool checkProceduralPreconditions() override;
    void activateAction() override;
};

GetPick::GetPick()
{
    mNeedsTarget = true;

    addPrecondition(STATE_HAS_PICK, false);
    addPrecondition(STATE_PICK_AVAILABLE, true);

    addEffect(STATE_HAS_PICK, true);
    addEffect(STATE_PICK_AVAILABLE, false);
}

bool GetPick::checkProceduralPreconditions()
{
    // Find mine
    std::weak_ptr<const Chest> chest = goap::entityCollection.findEntityByType<Chest>();

    // Store target
    mTarget = chest;

    return true;
}

void GetPick::activateAction()
{
    // Add pick to inventory
    // Get actor's inventory
    std::shared_ptr<Inventory> actorInv = mActor->findComponentByType<Inventory>().lock();

    // Remove pick
    actorInv->addItem(ITEM_PICKAXE, 1);

    // Remove pick from the chest
    // Get nearest chest's inventory
    std::shared_ptr<Chest> chest = std::dynamic_pointer_cast<Chest>(mTarget.lock());
    std::shared_ptr<Inventory> chestInv = chest->findComponentByType<Inventory>().lock();

    chestInv->removeItem(ITEM_PICKAXE, 1);
}

#endif /* GET_PICK_H */