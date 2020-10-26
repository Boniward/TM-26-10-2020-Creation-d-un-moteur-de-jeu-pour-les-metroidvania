#include "EntityReward.hpp"

EntityReward::EntityReward(std::string entity, void (*addEntity)(std::string)) :
m_entity(entity),
m_addEntity(addEntity)
{
}

EntityReward::~EntityReward()
{
}

void EntityReward::obtain()
{
    (*m_addEntity)(m_entity);
}

void EntityReward::listInfos()
{
    std::cout << m_entity << std::endl;
}
