#include "rendering/Rendering.h"

#include <SFML/Graphics/RectangleShape.hpp>

void renderGrid(sf::RenderTarget& renderTarget, int cellWidth,
    int cellHeight, int cellCountX, int cellCountY, sf::Color colour)
{
    for (int y = 0; y < cellCountX; ++y)
    {
        for (int x = 0; x < cellCountY; ++x)
        {
            // Render vertical line
            float x1 = (float)(x * cellWidth) + 0.1f;  /* otherwise it can't be seen
                                                       on the edge for some reason */
            float y1 = 0.0f;
            float x2 = x1;
            float y2 = (float)(cellHeight * cellCountY);

            renderLine(renderTarget, x1, y1, x2, y2, colour);
        }

        // Render horizontal line
        float x1 = 0.0f;
        float y1 = (float)(y * cellHeight);
        float x2 = (float)(cellWidth * cellCountX);
        float y2 = y1;

        renderLine(renderTarget, x1, y1, x2, y2, colour);
    }

    // Render right-most line
    float x1 = (float)(cellWidth * cellCountX);
    float y1 = 0.0f;
    float x2 = x1;;
    float y2 = (float)(cellHeight * cellCountY);

    renderLine(renderTarget, x1, y1, x2, y2, colour);

    // Render bottom-most line
    x1 = 0.0f;
    y1 = (float)(cellHeight * cellCountY);
    x2 = (float)(cellWidth * cellCountX);
    y2 = y1;

    renderLine(renderTarget, x1, y1, x2, y2, colour);
}

void renderLine(sf::RenderTarget& renderTarget, float x1,
    float y1, float x2, float y2, sf::Color colour)
{
    // Render horizontal line
    sf::Vertex line[2];
    line[0].position.x = x1;
    line[0].position.y = y1;
    line[1].position.x = x2;
    line[1].position.y = y2;
    line[0].color = colour;
    line[1].color = colour;

    renderTarget.draw(line, 2, sf::PrimitiveType::LinesStrip);
}

void renderRect(sf::RenderTarget& renderTarget, int x, int y,
    int width, int height, sf::Color colour)
{
    // Render rect
    sf::RectangleShape renderRect;
    renderRect.setFillColor(colour);
    renderRect.setSize(sf::Vector2f((float)width, (float)height));
    renderRect.setPosition((float)x, (float)y);

    renderTarget.draw(renderRect);
}