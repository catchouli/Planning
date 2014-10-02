#ifndef TESTPROJECT_SMELTERY_H
#define TESTPROJECT_SMELTERY_H

#include <entity/Entity.h>

class Smeltery
	: public goap::Entity
{
public:
    Smeltery(const glm::vec2& pos = glm::vec2());

	void update(float dt) override {}
    void render(void* renderer) override;

private:
    sf::Font mFont;
    sf::RectangleShape mRect;
};

Smeltery::Smeltery(const glm::vec2& pos)
    : goap::Entity(pos)
{
    // Load font
    mFont.loadFromFile("impact.ttf");

    // Set up render rect
    mRect.setSize(sf::Vector2f(32, 32));
    mRect.setFillColor(sf::Color::Color(rand(), rand(), rand()));
}

void Smeltery::render(void* renderer)
{
    sf::RenderWindow* renderWindow = (sf::RenderWindow*)renderer;

    // Draw body
    mRect.setPosition(mPosition.x, mPosition.y);
    renderWindow->draw(mRect);

    // Write contents
    sf::Text text;
    text.setFont(mFont);
    text.setColor(sf::Color::White);

    text.setCharacterSize(16);

    text.setPosition(mPosition.x + 40.0f, mPosition.y);
    text.setString("Smeltery");
    renderWindow->draw(text);
}

#endif /* TESTPROJECT_SMELTERY_H */