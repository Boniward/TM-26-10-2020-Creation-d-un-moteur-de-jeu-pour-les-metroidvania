#include "GateComponent.hpp"

int GateComponent::m_cooldown(1000);
int GateComponent::m_lastChange(0);

GateComponent::GateComponent(Entity* owner, std::string destination, unsigned int x, unsigned int y) :
Component(owner),
m_destination(destination),
m_actived(false),
m_newPosition{x, y}
{
}

GateComponent::~GateComponent()
{
}

void GateComponent::update()
{
}

void GateComponent::render()
{
}

void GateComponent::setActivity()
{
    m_actived = !m_actived;
}

bool GateComponent::getActivity() const
{
    return m_actived;
}

std::string GateComponent::getDestination() const
{
    return m_destination;
}

unsigned int GateComponent::getPosition(bool index) const
{
    return m_newPosition[index];
}

void GateComponent::enter()
{
    if(SDL_GetTicks() > m_lastChange + m_cooldown)
    {
        if(ColliderComponent::AABB(EntityManager::getPlayer()->getComponent<ColliderComponent>()->getHitbox(), m_owner->getComponent<ColliderComponent>()->getHitbox()))
        {
            m_actived = true;
            m_lastChange = SDL_GetTicks();
        }
    }
}
