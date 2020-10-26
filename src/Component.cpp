#include "Component.hpp"

Component::Component()
{
}

Component::Component(Entity* owner) : m_owner(owner)
{
}

Component::~Component()
{
    delete m_owner;
    m_owner = nullptr;
}
