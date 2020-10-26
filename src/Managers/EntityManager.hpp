#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <string>
#include <vector>
#include <algorithm>
#include "../Entity.hpp"
#include "../components/InformationComponent.hpp"

struct
{
    bool operator()(Entity* a, Entity* b) const
    {
        return a->getComponent<InformationComponent>()->getType() < b->getComponent<InformationComponent>()->getType();
    }
}EntitySorter;

class EntityManager
{
    public:
        EntityManager();
        ~EntityManager();

        void update();
        void render();

        void addEntity(std::string name);

        void deleteEntity(unsigned int index);
        void deleteEntities(EntityType type);
        void clear();

        Entity* getEntity(std::string name) const;
        static std::vector<Entity*> getEntities();
        static std::vector<Entity*> getEntities(EntityType type);
        static Entity* getPlayer();

        void sortEntities();
        void listEntities();

    private:
        static std::vector<Entity*> m_entities;
        static Entity* m_player;
};

#endif
