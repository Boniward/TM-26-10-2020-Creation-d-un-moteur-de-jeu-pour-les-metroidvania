#include "AdvancedEnemyComponent.hpp"

AdvancedEnemyComponent::AdvancedEnemyComponent(Entity* owner, int rangeX, int rangeY) :
Component(owner),
m_rangeX(rangeX * SCALE * TILE_SIZE),
m_rangeY(rangeY * SCALE * TILE_SIZE),
m_lastAttack(0),
m_lastDirection(true)
{
}

AdvancedEnemyComponent::~AdvancedEnemyComponent()
{
}

void AdvancedEnemyComponent::update()
{
    StatisticsComponent* statistics(m_owner->getComponent<StatisticsComponent>());
    ColliderComponent* collider(m_owner->getComponent<ColliderComponent>());
    SpriteComponent* sprite(m_owner->getComponent<SpriteComponent>());
    AttackComponent* attack(m_owner->getComponent<AttackComponent>());

    int ownerX(collider->getHitbox().x);
    int ownerW(collider->getHitbox().w);
    int attackX(attack->getAttackBox(m_lastDirection)->getHitbox().x);
    int attackW(attack->getAttackBox(m_lastDirection)->getHitbox().w);
    int playerX(EntityManager::getPlayer()->getComponent<ColliderComponent>()->getHitbox().x);
    int playerW(EntityManager::getPlayer()->getComponent<ColliderComponent>()->getHitbox().w);



    // Si l'ennemi est blessé, il a possibilité que la texture ne s'affiche pas. (c'est pour créer le clignotement)
    if(statistics->getHit() && (SDL_GetTicks() / 80 % 2))
    {
        sprite->setActivity(false);
    }
    else
    {
        sprite->setActivity(true);
    }

    // Si une animation linéaire est en cours
    if(sprite->getCurrentSpriteType() == LINEAR_SPRITE)
    {
        if(4 <= sprite->getCurrentFrameIndex().first && sprite->getCurrentFrameIndex().first <= 5)
        {
            if(attack->getFrameAttack() <= sprite->getCurrentFrameIndex().second && sprite->getCurrentFrameIndex().second <= attack->getFrameAttack()+1)
            {
                attack->attack(m_lastDirection);
            }
        }

        return;
    }

    else if(isPLayerInRange())
    {
        if(ColliderComponent::AABB(m_owner->getComponent<AttackComponent>()->getAttackBox(m_lastDirection)->getHitbox(), EntityManager::getPlayer()->getComponent<ColliderComponent>()->getHitbox()))
        {
            //m_owner->getComponent<MovementComponent>()->idle();

            if((SDL_GetTicks() > m_lastAttack + m_owner->getComponent<AttackComponent>()->getCooldown()))
            {
                m_lastAttack = SDL_GetTicks();
                m_owner->getComponent<MovementComponent>()->idle();

                if(m_owner->getComponent<AttackComponent>()->execute(m_lastDirection))
                {

                    m_owner->getComponent<SpriteComponent>()->setAction(4 + m_lastDirection);
                }
                else
                {
                    m_owner->getComponent<SpriteComponent>()->setAction(m_lastDirection);
                }
            }
        }

        else if(attackX + attackW <= playerX)
        {
            m_lastDirection = false;

            if(ownerX + ownerW <= playerX)
            {
                m_owner->getComponent<MovementComponent>()->move(m_lastDirection, false);
                m_owner->getComponent<SpriteComponent>()->setAction(2 + m_lastDirection);
            }
            else //if(playerX < ownerX + ownerW)
            {
                m_owner->getComponent<MovementComponent>()->idle();
                m_owner->getComponent<SpriteComponent>()->setAction(m_lastDirection);

            }
            /*else if(ownerX < playerX)
            {

            }*/
        }

        else if(playerX + playerW <= attackX)
        {
            m_lastDirection = true;

            if(playerX + playerW <= ownerX)
            {

                m_owner->getComponent<MovementComponent>()->move(m_lastDirection, false);
                m_owner->getComponent<SpriteComponent>()->setAction(2 + m_lastDirection);
            }
            else //if(ownerX < playerX + playerW)
            {
                m_owner->getComponent<MovementComponent>()->idle();
                m_owner->getComponent<SpriteComponent>()->setAction(m_lastDirection);
            }
            /*else if(playerX + playerW < ownerX + ownerW)
            {

            }*/
        }
    }

    else
    {
        m_owner->getComponent<MovementComponent>()->idle();
        m_owner->getComponent<SpriteComponent>()->setAction(m_lastDirection);
    }
}

void AdvancedEnemyComponent::render()
{

}

bool AdvancedEnemyComponent::isPLayerInRange()
{

    int playerPositionX(EntityManager::getPlayer()->getComponent<ColliderComponent>()->getHitbox().x + EntityManager::getPlayer()->getComponent<ColliderComponent>()->getHitbox().w/2);
    int playerPositionY(EntityManager::getPlayer()->getComponent<ColliderComponent>()->getHitbox().y + EntityManager::getPlayer()->getComponent<ColliderComponent>()->getHitbox().h/2);

    int ownerPositionX(m_owner->getComponent<ColliderComponent>()->getHitbox().x + m_owner->getComponent<ColliderComponent>()->getHitbox().w/2);
    int ownerPositionY(m_owner->getComponent<ColliderComponent>()->getHitbox().y + m_owner->getComponent<ColliderComponent>()->getHitbox().h/2);


    if(ownerPositionX - m_rangeX > playerPositionX)
    {
        return false;
    }
    if(playerPositionX > ownerPositionX + m_rangeX)
    {
        return false;
    }
    if(ownerPositionY - m_rangeY > playerPositionY)
    {
        return false;
    }
    if(playerPositionY > ownerPositionY + m_rangeY)
    {
        return false;
    }

    return true;
}
