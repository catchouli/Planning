#ifndef MAKE_AXE_H
#define MAKE_AXE_H

#include <entity/Entity.h>
#include <goap/Action.h>

#include "../data/WorldStates.h"
#include "../data/ItemIds.h"

#include "../entities/objects/Smeltery.h"

class MakeAxe
    : public goap::Action
{
public:
    MakeAxe();

    bool checkProceduralPreconditions() override;
    void activateAction() override;
};

MakeAxe::MakeAxe()
{
    mNeedsTarget = true;

    addPrecondition(STATE_HAS_WOOD, true);
    addPrecondition(STATE_HAS_ORE, true);

    addEffect(STATE_HAS_ORE, false);
    addEffect(STATE_HAS_WOOD, false);
    addEffect(STATE_HAS_AXE, true);
}

bool MakeAxe::checkProceduralPreconditions()
{
    // Find mine
    std::weak_ptr<Smeltery> smeltery = goap::entityCollection.findEntityByType<Smeltery>();

    // Store target
    mTarget = smeltery;

    return true;
}

void MakeAxe::activateAction()
{
    // Add pick to inventory and remove ore
    // Get actor's inventory
    std::shared_ptr<Inventory> actorInv = mActor->findComponentByType<Inventory>().lock();

    actorInv->addItem(ITEM_AXE, 1);
    actorInv->removeItem(ITEM_ORE, 1);
}

#endif /* MAKE_AXE_H */