#ifndef TESTPROJECT_WORKER_H
#define TESTPROJECT_WORKER_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <entity/GoapEntity.h>

#include "../objects/Inventory.h"

#include "../../data/ItemIds.h"

class Worker
    : public goap::GoapEntity
{
public:
    Worker(const glm::vec2& pos = glm::vec2(), const char* name = "Worker");

    virtual void update(float dt) override = 0;
    virtual void render(void* renderer) override;

    virtual goap::StateCollection getGoals() const override = 0;

    virtual goap::StateCollection getWorldState() const override = 0;

protected:
    std::shared_ptr<Inventory> mInventory;

    sf::Font mFont;
    sf::RectangleShape mRect;

    std::string mName;
};

Worker::Worker(const glm::vec2& pos, const char* name)
    : goap::GoapEntity(pos), mName(name)
{
    // Load font
    mFont.loadFromFile("impact.ttf");

    // Set up render rect
    mRect.setSize(sf::Vector2f(32, 32));
    mRect.setFillColor(sf::Color::Color(rand(), rand(), rand()));

    // Add inventory
    mInventory = addComponent<Inventory>().lock();
}

void Worker::update(float dt)
{
    // Do planning updates
    GoapEntity::update(dt);
}

void Worker::render(void* renderer)
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
    text.setString(mName);
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

#endif /* TESTPROJECT_WORKER_H */