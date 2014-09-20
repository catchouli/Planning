#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Actor.h"
#include "WorldStates.h"
#include "goap/Planning.h"

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

int main(int argc, char** argv)
{
	// Create window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Planning test");

	// Create world state
	StateCollection worldStateCollection;
	worldStateCollection[MINER_HAS_PICK] = true;

	// Actions
	Action mine(glm::vec2(50, 300), "mine");
	mine.addPre(MINER_HAS_PICK, true);
	mine.addPost(MINER_HAS_ORE, true);
	mine.addPost(MINER_HAS_PICK, false);

	Action getPick(glm::vec2(240, 50), "get pick");
	getPick.addPre(MINER_HAS_PICK, false);
	getPick.addPre(PICK_ON_TABLE, true);
	getPick.addPost(MINER_HAS_PICK, true);
	getPick.addPost(PICK_ON_TABLE, false);

	Action makePick(glm::vec2(500, 250), "make pick");
	makePick.addPre(SMITH_HAS_PICK, false);
	makePick.addPre(SMITH_HAS_ORE, true);
	makePick.addPost(SMITH_HAS_ORE, false);
	makePick.addPost(SMITH_HAS_PICK, true);

	Action storePick(glm::vec2(260, 50), "store pick");
	storePick.addPre(SMITH_HAS_PICK, true);
	storePick.addPost(PICK_ON_TABLE, true);
	storePick.addPost(SMITH_HAS_PICK, false);

	Action getOre(glm::vec2(260, 50), "get ore");
	getOre.addPre(SMITH_HAS_ORE, false);
	getOre.addPre(ORE_ON_TABLE, true);
	getOre.addPost(ORE_ON_TABLE, false);
	getOre.addPost(SMITH_HAS_ORE, true);

	Action storeOre(glm::vec2(240, 50), "store ore");
	storeOre.addPre(MINER_HAS_ORE, true);
	storeOre.addPost(ORE_ON_TABLE, true);
	storeOre.addPost(MINER_HAS_ORE, false);

	// Create actors
	std::vector<Actor> actors;

	// Create smith
	actors.push_back(Actor(glm::vec2(rand() % 800, rand() % 600), worldStateCollection));
	Actor& smith = actors.back();
	smith.addToGoal(PICK_ON_TABLE, true);
	smith.addPossibleAction(makePick);
	smith.addPossibleAction(storePick);
	smith.addPossibleAction(getOre);

	actors.push_back(Actor(glm::vec2(rand() % 800, rand() % 600), worldStateCollection));
	Actor& miner = actors.back();
	miner.addToGoal(ORE_ON_TABLE, true);
	miner.addPossibleAction(mine);
	miner.addPossibleAction(getPick);
	miner.addPossibleAction(storeOre);

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

		// Update actors
		for (auto& actor : actors)
		{
			actor.update(dt.asSeconds());
		}

		// Render
		window.clear();

		// Render actors
		for (auto& actor : actors)
		{
			actor.render(window);
		}

		window.display();
	}

	return 0;
}