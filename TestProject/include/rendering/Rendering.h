#ifndef TESTPROJECT_RENDERING_H
#define TESTPROJECT_RENDERING_H

#include <SFML/Graphics/RenderTarget.hpp>

void renderLine(sf::RenderTarget& renderTarget, float x1,
    float y1, float x2, float y2, sf::Color colour);
void renderGrid(sf::RenderTarget& renderTarget, int cellWidth,
    int cellHeight, int cellCountX, int cellCountY, sf::Color colour);
void renderRect(sf::RenderTarget& renderTarget, int x, int y,
    int width, int height, sf::Color colour);

#endif /* TESTPROJECT_RENDERING_H */