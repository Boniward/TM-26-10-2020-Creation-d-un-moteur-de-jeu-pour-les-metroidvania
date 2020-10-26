#include "MovementComponent.hpp"

MovementComponent::MovementComponent(Entity* owner, unsigned int speed) : Component(owner), m_speed(speed)
{
}

MovementComponent::~MovementComponent()
{
}

void MovementComponent::update()
{
}

void MovementComponent::render()
{
}

void MovementComponent::idle()
{
    m_owner->getComponent<TransformComponent>()->setVelocity(0, 0);
}

void MovementComponent::move(bool direction, bool sprint)
{
    m_owner->getComponent<TransformComponent>()->setVelocity(m_speed * SCALE * pow(-1, direction) * pow(2, sprint), 0);
}

void MovementComponent::jump()
{
    m_owner->getComponent<TransformComponent>()->setVelocity(-(TILE_SIZE*SCALE)/4, 1);
}

void MovementComponent::dodge(bool direction)
{

}
