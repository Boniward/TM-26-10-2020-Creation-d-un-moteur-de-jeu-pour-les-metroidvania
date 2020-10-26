#include "EntityManager.hpp"
#include <iostream>

std::vector<Entity*> EntityManager::m_entities;
Entity* EntityManager::m_player;

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

void EntityManager::update()
{
    for(auto& entity : m_entities)
    {
        entity->update();
    }
}

void EntityManager::render()
{
    for(auto& entity : m_entities)
    {
        entity->render();
    }
}

void EntityManager::deleteEntity(unsigned int index)
{
    delete m_entities[index];
    m_entities.erase(m_entities.begin() + index);
}

void EntityManager::deleteEntities(EntityType type)
{
    unsigned int index(0);
    for(Entity* entity : m_entities)
    {
        if(entity->getComponent<InformationComponent>()->getType())
        {
            delete entity;
            m_entities.erase(m_entities.begin() + index);
        }
        index++;
    }
}

void EntityManager::clear()
{
    for(auto& entity : m_entities)
    {
        delete entity;
    }
    m_entities.clear();
}

void EntityManager::addEntity(std::string name)
{
    // Création de l'entité
    Entity* newEntity(new Entity(name));

    // Ajout dans la liste des entités
    m_entities.emplace_back(newEntity);

    if(name == "player")
    {
        m_player = newEntity;
    }
}

Entity* EntityManager::getEntity(std::string name) const
{
    for(auto& entity : m_entities)
    {
        if(entity->getName() == name)
        {
            return entity;
        }
    }
    std::cerr << "ERREUR : ID D'ENTITE INCORRECT" << std::endl;
    return nullptr;
}

std::vector<Entity*> EntityManager::getEntities()
{
    return m_entities;
}

std::vector<Entity*> EntityManager::getEntities(EntityType type)
{
    std::vector<Entity*> entities;

    for(Entity* entity : m_entities)
    {
        if(entity->getComponent<InformationComponent>()->getType() == type)
        {
            entities.emplace_back(entity);
        }
    }

    return entities;
}

Entity* EntityManager::getPlayer()
{
    return m_player;
}

void EntityManager::sortEntities()
{
    std::sort(m_entities.begin(), m_entities.end(), EntitySorter);
}

void EntityManager::listEntities()
{
    unsigned int count(0);
    for(auto& entity : m_entities)
    {
        std::cout << "[" << count << "]Entity : " << entity->getName() << std::endl;
        entity->listComponents();
        std::cout << std::endl;
        count++;
    }
}
