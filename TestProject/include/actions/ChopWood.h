#ifndef CHOP_WOOD_H
#define CHOP_WOOD_H

#include <entity/Entity.h>
#include <goap/Action.h>

#include "../data/WorldStates.h"
#include "../data/ItemIds.h"

#include "../entities/objects/ReallyBigTree.h"
#include "../entities/objects/Inventory.h"

class ChopWood
    : public goap::Action
{
public:
    ChopWood();

    bool checkProceduralPreconditions() override;
    void activateAction() override;
};

ChopWood::ChopWood()
{
    mNeedsTarget = true;

    addPrecondition(STATE_HAS_AXE, true);

    addEffect(STATE_HAS_WOOD, true);
}

bool ChopWood::checkProceduralPreconditions()
{
    // Get actor's inventory
    std::shared_ptr<Inventory> inv = mActor->findComponentByType<Inventory>().lock();

    // Fail if we don't have an axe
    if (inv->getItemCount(ITEM_AXE) == 0)
        return false;

    // Find tree
    std::weak_ptr<ReallyBigTree> tree = goap::entityCollection.findEntityByType<ReallyBigTree>();

    // Store target
    mTarget = tree;

    return true;
}

void ChopWood::activateAction()
{
    // Add wood to inventory
    // Get actor's inventory
    std::shared_ptr<Inventory> actorInv = mActor->findComponentByType<Inventory>().lock();

    actorInv->addItem(ITEM_WOOD, 1);

    // A 50% chance of breaking the actor's axe
    float randf = rand() / (float)RAND_MAX;
    if (randf < 0.1f)
    {
        actorInv->removeItem(ITEM_AXE, 1);
    }
}

#endif /* CHOP_WOOD_H */