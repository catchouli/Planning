#ifndef MAKE_PICK_H
#define MAKE_PICK_H

#include <entity/Entity.h>
#include <goap/Action.h>

#include "../data/WorldStates.h"
#include "../data/ItemIds.h"

#include "../entities/objects/Smeltery.h"

class MakePick
    : public goap::Action
{
public:
    MakePick();

    bool checkProceduralPreconditions() override;
    void activateAction() override;
};

MakePick::MakePick()
{
    mNeedsTarget = true;

    addPrecondition(STATE_HAS_ORE, true);
    addPrecondition(STATE_HAS_PICK, false);

    addEffect(STATE_HAS_ORE, false);
    addEffect(STATE_HAS_PICK, true);
}

bool MakePick::checkProceduralPreconditions()
{
    // Find mine
    std::weak_ptr<Smeltery> smeltery = goap::entityCollection.findEntityByType<Smeltery>();

    // Store target
    mTarget = smeltery;

    return true;
}

void MakePick::activateAction()
{
    // Add pick to inventory
    // Get actor's inventory
    std::shared_ptr<Inventory> actorInv = mActor->findComponentByType<Inventory>().lock();

    actorInv->addItem(ITEM_PICKAXE, 1);
}

#endif /* MAKE_PICK_H */