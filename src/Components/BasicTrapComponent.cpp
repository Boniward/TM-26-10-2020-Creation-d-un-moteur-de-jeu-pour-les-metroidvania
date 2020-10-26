#include "BasicTrapComponent.hpp"

BasicTrapComponent::BasicTrapComponent(Entity* owner) : Component(owner)
{
}

BasicTrapComponent::~BasicTrapComponent()
{
}

void BasicTrapComponent::update()
{
    m_owner->getComponent<AttackComponent>()->attack();
}

void BasicTrapComponent::render()
{
}
