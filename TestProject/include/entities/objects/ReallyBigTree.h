#ifndef TESTPROJECT_REALLY_BIG_TREE_H
#define TESTPROJECT_REALLY_BIG_TREE_H

#include <entity/Entity.h>

class ReallyBigTree
    : public goap::Entity
{
public:
    ReallyBigTree(const glm::vec2& pos = glm::vec2());

    void update(float dt) override {}
    void render(void* renderer) override;

private:
    sf::Font mFont;
    sf::RectangleShape mRect;
};

ReallyBigTree::ReallyBigTree(const glm::vec2& pos)
    : goap::Entity(pos)
{
    // Load font
    mFont.loadFromFile("impact.ttf");

    // Set up render rect
    mRect.setSize(sf::Vector2f(32, 32));
    mRect.setFillColor(sf::Color::Color(rand(), rand(), rand()));
}

void ReallyBigTree::render(void* renderer)
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
    text.setString("Really Big Tree");
    renderWindow->draw(text);
}

#endif /* TESTPROJECT_REALLY_BIG_TREE_H */