#include "Entity.hpp"
#include <iostream>

Entity::Entity() : m_name("special entity")
{
}

Entity::Entity(std::string name) : m_name(name)
{
}

Entity::~Entity()
{
    std::cout << "\tSuppression de l'entite " << m_name << " en cours..." << std::endl;

    m_components.clear();

    std::cout << "\tSUCCES !" << std::endl;
}

void Entity::update()
{
    for(m_iterator = m_components.begin(); m_iterator != m_components.end(); m_iterator++)
    {
        m_iterator->second->update();
    }
}

void Entity::render()
{
    for(m_iterator = m_components.begin(); m_iterator != m_components.end(); m_iterator++)
    {
        m_iterator->second->render();
    }
}

void Entity::listComponents()
{
    for(m_iterator = m_components.begin(); m_iterator != m_components.end(); m_iterator++)
    {
        std::cout << "\tComponent<"<< m_iterator->first->name() << ">" << std::endl;
    }
}

std::string Entity::getName()
{
    return m_name;
}
