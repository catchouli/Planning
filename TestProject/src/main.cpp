#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <queue>
#include <vector>
#include <unordered_set>

#include <glm/glm.hpp>

#include <SFML/Graphics.hpp>

#include <entity/Entity.h>
#include <goap/ValueTypes.h>
#include <util/StdExtensions.h>

#include "data/ItemIds.h"

#include "entities/agents/Miner.h"
#include "entities/agents/AxeSmith.h"
#include "entities/agents/PickaxeSmith.h"
#include "entities/agents/Lumberjack.h"

#include "entities/objects/Inventory.h"
#include "entities/objects/Chest.h"
#include "entities/objects/Mine.h"
#include "entities/objects/ReallyBigTree.h"
#include "entities/objects/Smeltery.h"

#include "rendering/Rendering.h"

using namespace goap;

#include <vector>
#include <functional>

class Node
{
public:

    Node() : mCumulativeCost(0) {}

    /* The custom hash function for */
    virtual size_t getHash() const = 0;

    /* The custom equality operator */
    virtual bool operator==(const Node& other) const = 0;

    /* The custom neighbour accessor */
    virtual std::vector<std::shared_ptr<Node>> getNeighbours() const = 0;

    /* The cost of traversal to this node */
    virtual int getCost() const = 0;

    /* The heuristic (estimated cost to get to target) */
    virtual int getHeuristic(std::weak_ptr<const Node> target) const = 0;

    /* Set the cumulative cost (see mCumulativeCost) */
    void setCumulativeCost(int cumulativeCost)
    {
        mCumulativeCost = cumulativeCost;
        mTotalCost = mHeuristicCost + mCumulativeCost;
    }

    /* Get the cumulative cost (see mCumulativeCost) */
    int getCumulativeCost() const
    {
        return mCumulativeCost;
    }

    /* Set the heuristic cost (see mHeuristicCost) */
    void setHeuristicCost(int heuristicCost)
    {
        mHeuristicCost = heuristicCost;
        mTotalCost = mHeuristicCost + mCumulativeCost;
    }

    /* Get the heuristic cost (see mHeuristicCost) */
    int getHeuristicCost() const
    {
        return mHeuristicCost;
    }

    /* Get the total cost (automatically calculated when the other costs are set) */
    int getTotalCost() const
    {
        return mTotalCost;
    }

    /* Set the parent of this node */
    void setParent(std::weak_ptr<Node> parent)
    {
        mParent = parent;
    }

    /* Get the parent of this node */
    std::weak_ptr<Node> getParent() const
    {
        return mParent;
    }

private:

    /* The parent of this node */
    std::weak_ptr<Node> mParent;

    /* Cumulative cost to get to this node on the shortest path found so far */
    int mCumulativeCost;

    /* Heuristic cost to get to the end from this node */
    int mHeuristicCost;

    /* Total cost (mCumulativeCost + mHeuristicCost) */
    int mTotalCost;

};

struct NodeCostComparatorGreater
{
    bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const
    {
        return a->getTotalCost() < b->getTotalCost();
    }
};

namespace std
{
    template <>
    struct std::hash<Node>
    {
        size_t operator()(const Node& node) const
        {
            return node.getHash();
        }
    };

    template<>
    bool operator==(const std::shared_ptr<const Node>& a, const std::shared_ptr<const Node>& b)
    {
        return a->operator==(*b);
    }
}

class Grid
{
public:

    /* Construct a grid of a fixed width and height */
    Grid(int width, int height)
        : width(width), height(height)
    {
        cells = new bool[width * height];
        memset(cells, 0, sizeof(bool) * width * height);
    }
    
    /* Deallocate grid memory (I can't wait for std::dynarray) */
    ~Grid()
    {
        delete cells;
    }
    
    /* Sort of like array indices grid[x][y] -> grid(x, y) */
    bool& operator()(int x, int y)
    {
        assert(x >= 0 && y >= 0 && x < width && y < height);

        // Accessing a cell at (x, y), index = y * width + x
        return cells[y * width + x];
    }

    /* The width and height of the array at cells */
    int width, height;

    /* Pointer to underlying array */
    bool* cells;

private:

    /* Prevent copy construction and assignment */
    Grid();
    Grid& operator=(const Grid& other);

};

class Cell
    : public Node
{
public:

    /* Public constructor */
    Cell(Grid* map, int x, int y, bool diagonals = true)
        : mMap(map), mLocation(x, y), mCost(0), mDiagonals(diagonals) {}

    /* Hash function based on the location */
    size_t getHash() const override
    {
        return std::hash<std::pair<int, int>>()(mLocation);
    }

    /* Equality operator to compare position */
    bool operator==(const Node& other) const
    {
        // Get the cell for this node
        const Cell* otherCell = static_cast<const Cell*>(&other);

        // Check that this is actually a cell
        assert(otherCell != nullptr);

        return mLocation == otherCell->mLocation;
    }

    /* Get the unblocked neighbours from the map */
    std::vector<std::shared_ptr<Node>> getNeighbours() const override
    {
        std::vector<std::shared_ptr<Node>> neighbours;

        // Check the position of the cell is is valid and add it to neighbours
        auto addIfUnblocked = [&](Grid* mMap, int x, int y, int cost)
        {
            if (x >= 0 && x < mMap->width && y >= 0 && y < mMap->height)
            {
                if (!mMap->operator()(x, y))
                    neighbours.push_back(std::shared_ptr<Cell>(new Cell(mMap, x, y, cost, mDiagonals)));
            }
        };

        // Get position of cell
        int x = mLocation.first;
        int y = mLocation.second;

        // Add unblocked cells
        addIfUnblocked(mMap, x + 1, y,      1);
        addIfUnblocked(mMap, x - 1, y,      1);
        addIfUnblocked(mMap, x,     y + 1,  1);
        addIfUnblocked(mMap, x,     y - 1,  1);

        if (mDiagonals)
        {
            addIfUnblocked(mMap, x - 1, y - 1, 2);
            addIfUnblocked(mMap, x - 1, y + 1, 2);
            addIfUnblocked(mMap, x + 1, y - 1, 2);
            addIfUnblocked(mMap, x + 1, y + 1, 2);
        }

        return neighbours;
    }

    /* Get the cost as defined by the construction in getNeighbours (10 cardinal, 14 diagonal) */
    int getCost() const override
    {
        return mCost;
    }

    /* Calculate the heuristic (manhattan) */
    int getHeuristic(std::weak_ptr<const Node> target) const override
    {
        // Get pointer
        std::shared_ptr<const Cell> targetCell = std::dynamic_pointer_cast<const Cell>(target.lock());

        // Check that this node is a cell
        assert(targetCell != nullptr);

        // Get square distance to target
        float heuristic;
        
        int dx = abs(mLocation.first - targetCell->mLocation.first);
        int dy = abs(mLocation.second - targetCell->mLocation.second);

        return 10 * (dx + dy);
    }

    /* Getters for location */
    int getX() const
    {
        return mLocation.first;
    }

    int getY() const
    {
        return mLocation.second;
    }
    
protected:

    /* Internal constructor used by getNeighbours to set the cost */
    Cell(Grid* map, int x, int y, int cost, bool diagonals)
        : mMap(map), mLocation(x, y), mCost(cost), mDiagonals(diagonals) {}

    /* The map this cell is from */
    Grid* mMap;

    /* Whether diagonals are valid */
    bool mDiagonals;

    /* The cost of traversal to this node */
    int mCost;

    /* The location of this node */
    std::pair<int, int> mLocation;

};

std::vector<std::shared_ptr<Node>> search(std::shared_ptr<Node> start, std::shared_ptr<Node> end);

int main(int argc, char** argv)
{
    bool value = FloatValue(true) == FloatValue(false);
    printf("%s\n", value ? "true" : "false");

    srand(2);

    // Map data
    const int MAP_WIDTH = 50;
    const int MAP_HEIGHT = 50;

    const int CELL_WIDTH = 10;
    const int CELL_HEIGHT = 10;

    // The start and end for pathfinding
    glm::ivec2 ptStart(0, 0);
    glm::ivec2 ptEnd(MAP_WIDTH - 1, MAP_HEIGHT - 1);

    // The pathfinding map
    Grid map(MAP_WIDTH, MAP_HEIGHT);
    map(5, 6) = true;

    // Node min heap
    std::priority_queue<std::shared_ptr<Node>,
        std::vector<std::shared_ptr<Node>>,
        NodeCostComparatorGreater> nodeMinHeap;

    auto c1 = std::make_shared<Cell>(&map, rand() % 16, rand() % 16);
    auto c2 = std::make_shared<Cell>(&map, rand() % 16, rand() % 16);
    auto c3 = std::make_shared<Cell>(&map, rand() % 16, rand() % 16);
    auto c4 = std::make_shared<Cell>(&map, rand() % 16, rand() % 16);

    std::set<std::shared_ptr<Node>, NodeCostComparatorGreater> openSet;
    openSet.insert(c3);
    openSet.insert(c1);
    openSet.insert(c4);
    openSet.insert(c2);
    
    auto test = *openSet.begin();

    // Test
    std::vector<int> intArray;

    for (int i = 0; i < 100; ++i)
    {
        intArray.push_back(rand());
    }

	// Create entities
    // Agents
    //goap::entityCollection.createEntity<Miner>(glm::vec2(50, 50));
    //goap::entityCollection.createEntity<Lumberjack>(glm::vec2(250, 130));
    //goap::entityCollection.createEntity<AxeSmith>(glm::vec2(100, 100));
    //goap::entityCollection.createEntity<PickaxeSmith>(glm::vec2(75, 175));

    // Objects
    //goap::entityCollection.createEntity<Chest>(glm::vec2(250, 250));
    //goap::entityCollection.createEntity<Mine>(glm::vec2(50, 500));
    //goap::entityCollection.createEntity<ReallyBigTree>(glm::vec2(350, 50));
	//goap::entityCollection.createEntity<Smeltery>(glm::vec2(500, 350));
	
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

        // Update map
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i pos = sf::Mouse::getPosition(window);

            int curTileX = pos.x / CELL_WIDTH;
            int curTileY = pos.y / CELL_HEIGHT;

            if (curTileX >= 0 && curTileY >= 0 && curTileX < MAP_WIDTH && curTileY < MAP_HEIGHT)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                {
                    map(curTileX, curTileY) = true;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                {
                    map(curTileX, curTileY) = false;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                {
                    ptStart = glm::ivec2(curTileX, curTileY);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F))
                {
                    ptEnd = glm::ivec2(curTileX, curTileY);
                }
            }
        }

        // Search map
        std::vector<std::shared_ptr<Node>> path =
            search(std::make_shared<Cell>(&map, ptStart.x, ptStart.y, false),
            std::make_shared<Cell>(&map, ptEnd.x, ptEnd.y, true));

		// Update entities
		for (auto& entity : goap::entityCollection)
		{
			entity->update(dt.asSeconds());
		}

		// Render
		window.clear();

        // Render map
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            for (int x = 0; x < MAP_WIDTH; ++x)
            {
                if (map(x, y))
                {
                    // Render rect
                    renderRect(window, x * CELL_WIDTH, y * CELL_HEIGHT,
                        CELL_WIDTH, CELL_HEIGHT, sf::Color::White);
                }
            }
        }

        // Render path-finding start and end
        renderRect(window, ptStart.x * CELL_WIDTH, ptStart.y * CELL_HEIGHT,
            CELL_WIDTH, CELL_HEIGHT, sf::Color::Green);
        renderRect(window, ptEnd.x * CELL_WIDTH, ptEnd.y * CELL_HEIGHT,
            CELL_WIDTH, CELL_HEIGHT, sf::Color::Red);

        // Render path
        // Colour lerp
        float cellCount = (float)path.size();
        glm::vec3 startColour(0.0f, 255.0f, 0.0f);
        glm::vec3 endColour(255.0f, 0.0f, 0.0f);
        glm::vec3 stepDiff = (endColour - startColour) / (cellCount - 1);
        
        glm::vec3 colour = startColour;

        for (const auto& node : path)
        {
            std::shared_ptr<const Cell> cell = std::dynamic_pointer_cast<const Cell>(node);

            // Render rect
            renderRect(window, cell->getX() * CELL_WIDTH, cell->getY() * CELL_HEIGHT,
                CELL_WIDTH, CELL_HEIGHT, sf::Color((int)colour.r, (int)colour.g, (int)colour.b));

            colour += stepDiff;
        }

        // Render grid
        renderGrid(window, CELL_WIDTH, CELL_HEIGHT, MAP_WIDTH, MAP_HEIGHT, sf::Color::White);

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

std::vector<std::shared_ptr<Node>> search(std::shared_ptr<Node> start, std::shared_ptr<Node> end)
{
    // Open and closed lists (sorted by overall cost with the lowest at the start)
    std::vector<std::shared_ptr<Node>> closed;
    std::vector<std::shared_ptr<Node>> open;

    // Add the starting node to the open list
    start->setHeuristicCost(start->getHeuristic(end));
    open.push_back(start);

    int i = 0;

    while (open.size() > 0)
    {
        i++;

        // Get lowest overall cost node from open list (the first one)
        std::sort(open.begin(), open.end(), [](std::shared_ptr<Node> a, std::shared_ptr<Node> b) { return a->getTotalCost() < b->getTotalCost(); });
        std::shared_ptr<Node> current = *open.begin();
        open.erase(open.begin());

        // Switch it to the closed list
        closed.push_back(current);

        // If the node we just pushed back is the target node, we found the path
        if (current->operator==(*end))
            break;

        // For each adjacent node
        for (auto adjacent : current->getNeighbours())
        {
            auto findAdjacent = [&](const std::shared_ptr<Node>& a) { return a->operator==(*adjacent); };
            
            // Calculate cumulative cost
            int cumulativeCost = current->getCumulativeCost() + adjacent->getCost();

            // Ignore this node if it's already on the closed list
            auto it = std::find_if(closed.begin(), closed.end(), findAdjacent);
            if (it != closed.end())
                continue;

            // If it isn't on the open list
            auto openListEntry = std::find_if(open.begin(), open.end(), findAdjacent);
            if (openListEntry == open.end())
            {
                // Add it, setting its parent
                adjacent->setParent(current);
                adjacent->setCumulativeCost(cumulativeCost);
                adjacent->setHeuristicCost(adjacent->getHeuristic(end));
                open.push_back(adjacent);
            }
            else
            {
                // Check if the cost is greater than the cost on this path
                if (cumulativeCost < (*openListEntry)->getCumulativeCost())
                {
                    // Remove this entry and insert the new one with correct parent and new cost
                    (*openListEntry)->setParent(current);
                    (*openListEntry)->setCumulativeCost(cumulativeCost);
                }
            }
        }
    }

    // Follow backwards from target
    std::vector<std::shared_ptr<Node>> path;

    auto findEnd = [&](const std::shared_ptr<Node>& a) { return a->operator==(*end); };

    // Ignore this node if it's already on the closed list
    auto target = std::find_if(closed.begin(), closed.end(), findEnd);

    // If the end is on the closed list (if we found it)
    if (target != closed.end())
    {
        // Follow parents back to end to get reverse of path
        std::shared_ptr<Node> node = *target;

        while (node)
        {
            path.push_back(node);

            node = node->getParent().lock();
        }

        // Reverse path
        std::reverse(path.begin(), path.end());
    }

    return closed;
}