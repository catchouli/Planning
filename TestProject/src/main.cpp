#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

#include <entity/Entity.h>

#include "entities/agents/Miner.h"
#include "entities/agents/Smith.h"
#include "entities/objects/Inventory.h"
#include "entities/objects/Chest.h"
#include "entities/objects/Mine.h"
#include "entities/objects/Smeltery.h"

#include "data/ItemIds.h"

using namespace goap;

int main(int argc, char** argv)
{
	// Create entities
    // Agents
    std::weak_ptr<Miner> miner = goap::entityCollection.createEntity<Miner>(glm::vec2(50, 50));
    std::weak_ptr<Smith> smith = goap::entityCollection.createEntity<Smith>(glm::vec2(100, 100));

    // Objects
	goap::entityCollection.createEntity<Chest>(glm::vec2(250, 250));
	goap::entityCollection.createEntity<Mine>(glm::vec2(50, 500));
	goap::entityCollection.createEntity<Smeltery>(glm::vec2(500, 350));
	
	// Create window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Planning test");

	// Timer
	sf::Clock deltaClock;

	// Main loop
	while (window.isOpen())
	{
		// Get delta time
		sf::Time dt = deltaClock.restart();

		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed ||
				(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();
		}

		// Update entities
		for (auto& entity : goap::entityCollection)
		{
			entity->update(dt.asSeconds());
		}

		// Render
		window.clear();

		// Render entities
		for (auto& entity : goap::entityCollection)
		{
			entity->render(&window);
		}

		// Flip buffers
		window.display();
	}

	return 0;
}