#ifndef PLANNING_ENTITY_H
#define PLANNING_ENTITY_H

#include <vector>
#include <memory>
#include <type_traits>
#include <glm/glm.hpp>

namespace goap
{
    /**
     * The entity class of our entity system
     * update and render must be overidden when subclassing
     */
    class Entity
    {
    public:

        /* Default constructor */
        Entity();

        /* Construct entity with a position */
        Entity(const glm::vec2& position);

        /* Update and render functions must be implemented by the user*/
        virtual void update(float dt) = 0;
        virtual void render(void* renderer) = 0;

        /* Accessors and mutators for the entity's position*/
        glm::vec2& getPosition();
        const glm::vec2& getPosition() const;

        void setPosition(const glm::vec2& newPosition);

        /* Causes this entity to move towards a position, stopping at it */
        void moveTowards(const glm::vec2& position, float dist);

        /* Add a component to this entity */
        template <typename T, typename... Args>
        std::weak_ptr<T> addComponent(Args&&... args);

        /* Get a component of a type from this entity */
        template <typename T>
        std::weak_ptr<T> findComponentByType();

    protected:

        /* This entity's components */
        std::vector<std::shared_ptr<Entity>> mComponents;

        /* The position of the entity */
        glm::vec2 mPosition;
    };

    /**
     * A helper container for entities that allows entities to be directly constructed
     * and inserted into the container, returning a non-owning pointer
     */
    class EntityCollection
        : public std::vector<std::shared_ptr<Entity>>
    {
    public:
        template <typename T, typename... Args>
        std::weak_ptr<T> createEntity(Args&&... args);

        template <typename T>
        std::weak_ptr<T> findEntityByType();
    };

    /* A static entity container to enable easy access to our entities */
    static class EntityCollection entityCollection;

    /* Inline function definitions */

    inline glm::vec2& Entity::getPosition()
    {
        return mPosition;
    }

    inline const glm::vec2& Entity::getPosition() const
    {
        return mPosition;
    }

    inline void Entity::setPosition(const glm::vec2& newPosition)
    {
        mPosition = newPosition;
    }

    template <typename T, typename... Args>
    std::weak_ptr<T> Entity::addComponent(Args&&... args)
    {
        std::shared_ptr<T> ptr = std::make_shared<T>(args...);

        mComponents.push_back(ptr);
        return ptr;
    }

    template <typename T>
    std::weak_ptr<T> Entity::findComponentByType()
    {
        for (auto entity : mComponents)
        {
            auto ptr = std::dynamic_pointer_cast<T>(entity);

            if (ptr)
            {
                return ptr;
            }
        }

        return std::weak_ptr<T>();
    }

    template <typename T, typename... Args>
    std::weak_ptr<T> EntityCollection::createEntity(Args&&... args)
    {
        std::shared_ptr<T> ptr = std::make_shared<T>(args...);

        push_back(ptr);
        return ptr;
    }

    template <typename T>
    std::weak_ptr<T> EntityCollection::findEntityByType()
    {
        for (auto entity : *this)
        {
            auto ptr = std::dynamic_pointer_cast<T>(entity);

            if (ptr)
            {
                return ptr;
            }
        }

        return std::weak_ptr<T>();
    }
}

#endif /* PLANNING_ENTITY_H */