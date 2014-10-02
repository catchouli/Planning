#ifndef TESTPROJECT_CHEST_H
#define TESTPROJECT_CHEST_H

#include <entity/Entity.h>

#include "Inventory.h"

#include "../../data/ItemIds.h"

class Chest
	: public goap::Entity
{
public:
    Chest(const glm::vec2& pos = glm::vec2());

	void update(float dt) override {}
    void render(void* renderer) override;

private:
    std::shared_ptr<Inventory> mInventory;

    sf::Font mFont;
    sf::RectangleShape mRect;
};


inline Chest::Chest(const glm::vec2& pos)
    : goap::Entity(pos)
{
    // Load font
    mFont.loadFromFile("impact.ttf");

    // Set up render rect
    mRect.setSize(sf::Vector2f(32, 32));
    mRect.setFillColor(sf::Color::Color(rand(), rand(), rand()));

    // Add components
    // Inventory
    mInventory = addComponent<Inventory>().lock();
}

inline void Chest::render(void* renderer)
{
    sf::RenderWindow* renderWindow = (sf::RenderWindow*)renderer;

    // Draw body
    mRect.setPosition(mPosition.x, mPosition.y);
    renderWindow->draw(mRect);

    // Write contents
    sf::Text text;
    text.setFont(mFont);
    text.setCharacterSize(14);
    text.setColor(sf::Color::White);

    text.setPosition(mPosition.x + 40.0f, mPosition.y);
    text.setString("Chest contents:");
    renderWindow->draw(text);

    int i = 0;
    for (const auto& entry : mInventory->getItems())
    {
        char buf[1024];
        sprintf_s(buf, "%s x %d", ItemNames[entry.first], entry.second);

        text.setPosition(mPosition.x + 45.0f, mPosition.y + 20.0f + i * 20.0f);
        text.setString(buf);
        renderWindow->draw(text);

        i++;
    }
}

#endif /* TESTPROJECT_CHEST_H */