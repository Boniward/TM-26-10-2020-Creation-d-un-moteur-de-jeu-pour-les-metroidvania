#include "InformationComponent.hpp"

std::map<std::string, EntityType> entityTypes{
    {"background", BACKGROUD_TYPE},
    {"gate", GATE_TYPE},
    {"enemy", ENEMY_TYPE},
    {"trap", TRAP_TYPE},
    {"player", PLAYER_TYPE},
    {"ui", UI_TYPE}
};

InformationComponent::InformationComponent(Entity* owner, std::string id, std::string type) : Component(owner), m_id(id), m_type(entityTypes[type])
{
}

InformationComponent::~InformationComponent()
{
}

void InformationComponent::update()
{
}

void InformationComponent::render()
{
}

std::string InformationComponent::getId()
{
    return m_id;
}

EntityType InformationComponent::getType()
{
    return m_type;
}
