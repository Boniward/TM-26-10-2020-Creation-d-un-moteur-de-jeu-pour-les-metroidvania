#include "AttackComponent.hpp"

AttackComponent::AttackComponent(Entity* owner, int x, int y, int w, int h, unsigned int frameAttack, unsigned int cooldown) :
Component(owner),
m_spaceX(x),
m_spaceY(y),
m_lastAttack(0),
m_frameAttack(frameAttack),
m_cooldown(cooldown),
m_attackBox{new Entity()}
{
    m_attackBox->addComponent<TransformComponent>(m_owner->getComponent<TransformComponent>()->getPosition(0) + m_spaceX * x, m_owner->getComponent<TransformComponent>()->getPosition(1) + y * SCALE, w, h, 0, 0, 0, 0);
    m_attackBox->addComponent<ColliderComponent>(0, 0);
}

AttackComponent::~AttackComponent()
{
    delete m_attackBox;
}

void AttackComponent::update()
{

}

void AttackComponent::render()
{
}

unsigned int AttackComponent::getCooldown() const
{
    return m_cooldown;
}

ColliderComponent* AttackComponent::getAttackBox(bool direction)
{
    m_attackBox->getComponent<TransformComponent>()->setPosition(m_owner->getComponent<TransformComponent>()->getPosition(0) + direction * m_owner->getComponent<TransformComponent>()->getSize(0) - direction * m_attackBox->getComponent<TransformComponent>()->getSize(0) - pow(-1, !direction) * m_spaceX * SCALE, m_owner->getComponent<TransformComponent>()->getPosition(1) + m_spaceY * SCALE);
    return m_attackBox->getComponent<ColliderComponent>();
}

unsigned int AttackComponent::getFrameAttack() const
{
    return m_frameAttack;
}

bool AttackComponent::execute(bool direction)
{
    if((SDL_GetTicks() - m_lastAttack) > m_cooldown)
    {
        return true;
    }
    return false;
}

void AttackComponent::attack(bool direction)
{
    // Positionnement de la zone de dégât
    m_attackBox->getComponent<TransformComponent>()->setPosition(m_owner->getComponent<TransformComponent>()->getPosition(0) + direction * m_owner->getComponent<TransformComponent>()->getSize(0) - direction * m_attackBox->getComponent<TransformComponent>()->getSize(0) - pow(-1, !direction) * m_spaceX * SCALE, m_owner->getComponent<TransformComponent>()->getPosition(1) + m_spaceY * SCALE);

    // Recherche d'un ennemi possiblement touchable
    switch(m_owner->getComponent<InformationComponent>()->getType())
    {
        case PLAYER_TYPE:
            for(Entity* entity : EntityManager::getEntities(ENEMY_TYPE))
            {
                if(ColliderComponent::AABB(entity->getComponent<ColliderComponent>()->getHitbox(), m_attackBox->getComponent<ColliderComponent>()->getHitbox()) && !entity->getComponent<StatisticsComponent>()->getHit())
                {
                    entity->getComponent<StatisticsComponent>()->setHealth(m_owner->getComponent<StatisticsComponent>()->getDamage());
                    entity->getComponent<StatisticsComponent>()->setHit();
                }
            }
            break;

        case ENEMY_TYPE:
        case TRAP_TYPE:
            if(ColliderComponent::AABB(EntityManager::getPlayer()->getComponent<ColliderComponent>()->getHitbox(), m_attackBox->getComponent<ColliderComponent>()->getHitbox()) && !EntityManager::getPlayer()->getComponent<StatisticsComponent>()->getHit())
            {
                EntityManager::getPlayer()->getComponent<StatisticsComponent>()->setHealth(m_owner->getComponent<StatisticsComponent>()->getDamage());
                EntityManager::getPlayer()->getComponent<StatisticsComponent>()->setHit();
            }
            break;

        default:
            break;
    }
}
