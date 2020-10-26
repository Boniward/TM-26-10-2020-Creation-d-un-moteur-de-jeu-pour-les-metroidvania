#include "BasicEnemyComponent.hpp"

BasicEnemyComponent::BasicEnemyComponent(Entity* owner, int start, int distance, int speed) :
Component(owner),
m_roadA(start*TILE_SIZE*SCALE),
m_roadB((start+distance+1)*TILE_SIZE*SCALE - m_owner->getComponent<ColliderComponent>()->getHitbox().w),
m_speed(speed),
m_transform(m_owner->getComponent<TransformComponent>()),
m_sprite(m_owner->getComponent<SpriteComponent>())
{
}

BasicEnemyComponent::~BasicEnemyComponent()
{
}

void BasicEnemyComponent::update()
{
    if(m_owner->getComponent<TransformComponent>()->getPosition(0) <= m_roadA)
    {
        m_owner->getComponent<TransformComponent>()->setVelocity(m_speed, 0);
    }

    else if(m_owner->getComponent<TransformComponent>()->getPosition(0) >= m_roadB)
    {
        m_owner->getComponent<TransformComponent>()->setVelocity(-m_speed, 0);
    }

    m_owner->getComponent<AttackComponent>()->attack();

    if(m_owner->getComponent<StatisticsComponent>()->getHit() && (SDL_GetTicks() / 80 % 2))
    {
        m_owner->getComponent<SpriteComponent>()->setActivity(false);
    }
    else
    {
        m_owner->getComponent<SpriteComponent>()->setActivity(true);
    }
}

void BasicEnemyComponent::render()
{
}
