#ifndef TESTPROJECT_SMITH_H
#define TESTPROJECT_SMITH_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <entity/GoapEntity.h>

#include "../objects/Chest.h"

#include "../../actions/GetOre.h"
#include "../../actions/MakePick.h"
#include "../../actions/StorePick.h"

class Smith
    : public goap::GoapEntity
{
public:
    Smith(const glm::vec2& pos = glm::vec2());

    void update(float dt) override;
    void render(void* renderer) override;

    goap::StateCollection getGoals() const override;

    goap::StateCollection getWorldState() const override;

private:
    std::shared_ptr<Inventory> mInventory;

    sf::Font mFont;
    sf::RectangleShape mRect;
};

Smith::Smith(const glm::vec2& pos)
    : goap::GoapEntity(pos)
{
    // Load font
    mFont.loadFromFile("impact.ttf");

    // Set up render rect
    mRect.setSize(sf::Vector2f(32, 32));
    mRect.setFillColor(sf::Color::Color(rand(), rand(), rand()));

    // Add inventory
    mInventory = addComponent<Inventory>().lock();

    // Add possible actions
    addAction<GetOre>();
    addAction<MakePick>();
    addAction<StorePick>();
}

void Smith::update(float dt)
{
    // Do planning updates
    GoapEntity::update(dt);
}

void Smith::render(void* renderer)
{
    sf::RenderWindow* renderWindow = (sf::RenderWindow*)renderer;

    // Draw body
    mRect.setPosition(mPosition.x, mPosition.y);
    renderWindow->draw(mRect);

    // Draw current action
    std::shared_ptr<goap::Action> currentAction = getCurrentAction().lock();

    std::string currentActionName = "None";

    if (currentAction)
    {
        currentActionName = typeid(*currentAction).name();
    }

    sf::Text text;
    text.setFont(mFont);
    text.setColor(sf::Color::White);

    text.setCharacterSize(16);
    text.setPosition(mPosition.x + 40.0f, mPosition.y);
    text.setString("Smith");
    renderWindow->draw(text);

    text.setCharacterSize(14);

    text.setPosition(mPosition.x + 45.0f, mPosition.y + 20.0f);
    text.setString(sf::String("Current action: ") + currentActionName);
    renderWindow->draw(text);

    text.setPosition(mPosition.x + 45.0f, mPosition.y + 40.0f);
    text.setString("Inventory contents:");
    renderWindow->draw(text);

    int i = 0;
    for (const auto& entry : mInventory->getItems())
    {
        char buf[1024];
        sprintf_s(buf, "%s x %d", ItemNames[entry.first], entry.second);

        text.setPosition(mPosition.x + 50.0f, mPosition.y + 60.0f + i * 20.0f);
        text.setString(buf);
        renderWindow->draw(text);

        i++;
    }
}

goap::StateCollection Smith::getGoals() const
{
    goap::StateCollection goals;

    goals[STATE_PICK_AVAILABLE] = true;

    return goals;
}

goap::StateCollection Smith::getWorldState() const
{
    goap::StateCollection worldState;

    // Check if we have ore
    if (mInventory->getItemCount(ITEM_ORE) > 0)
    {
        worldState[STATE_HAS_ORE] = true;
    }

    // Check if we have a pickaxe
    if (mInventory->getItemCount(ITEM_PICKAXE) > 0)
    {
        worldState[STATE_HAS_PICK] = true;
    }

    // Check if there's ore / a pickaxe available in the first chest we find
    std::shared_ptr<Chest> chest = goap::entityCollection.findEntityByType<Chest>().lock();

    if (chest)
    {
        std::shared_ptr<Inventory> chestInv = chest->findComponentByType<Inventory>().lock();

        if (chestInv)
        {
            if (chestInv->getItemCount(ITEM_ORE) > 0)
            {
                worldState[STATE_ORE_AVAILABLE] = true;
            }

            if (chestInv->getItemCount(ITEM_PICKAXE) > 0)
            {
                worldState[STATE_PICK_AVAILABLE] = true;
            }
        }
    }

    return worldState;
}

#endif /* TESTPROJECT_SMITH_H */