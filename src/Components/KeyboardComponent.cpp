#include "KeyboardComponent.hpp"

KeyboardComponent::KeyboardComponent(Entity* owner, SDL_Scancode upKey, SDL_Scancode downKey, SDL_Scancode leftKey, SDL_Scancode rightKey, SDL_Scancode sprintKey, SDL_Scancode attackKey, SDL_Scancode actionKey) :
Component(owner),
m_upKey(upKey),
m_downKey(downKey),
m_leftKey(leftKey),
m_rightKey(rightKey),
m_sprintKey(sprintKey),
m_attackKey(attackKey),
m_actionKey(actionKey),
m_keys(Game::getKeys()),
m_lastDirection(true),
m_lastAttack(0)
{
}

void KeyboardComponent::update()
{
    // Récupération de certaines infos d'autres composants
    MovementComponent* movement = m_owner->getComponent<MovementComponent>();
    SpriteComponent* sprite = m_owner->getComponent<SpriteComponent>();
    ColliderComponent* collider = m_owner->getComponent<ColliderComponent>();
    AttackComponent* attack = m_owner->getComponent<AttackComponent>();

    // Si le joueur est blessé, il a possibilité que la texture ne s'affiche pas. (c'est pour créer le clignotement)
    if(m_owner->getComponent<StatisticsComponent>()->getHit() && ((SDL_GetTicks() / 80) % 2))
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
        if(8 <= sprite->getCurrentFrameIndex().first && sprite->getCurrentFrameIndex().first <= 9)
        {
            if(attack->getFrameAttack() <= sprite->getCurrentFrameIndex().second && sprite->getCurrentFrameIndex().second <= attack->getFrameAttack()+1)
            {
                attack->attack(m_lastDirection);
            }
        }

        return;
    }

    // sauter
    if(m_keys[m_upKey])
    {
        if(!collider->getFalling())
        {
            movement->jump();
            collider->setFalling();
        }
    }

    // se baisser
    if(m_keys[m_downKey])
    {
        //m_transform->setVelocity(5, 1);
    }

    // déplacement horizontal
    bool right(false);
    bool left(false);
    bool sprint(false);

    if(m_keys[m_rightKey])
    {
        right = true;
        sprite->setAction(2);
        m_lastDirection = false;
    }

    if(m_keys[m_leftKey])
    {
        left = true;
        sprite->setAction(3);
        m_lastDirection = true;
    }

    if(m_keys[m_sprintKey])
    {
        sprint = true;
    }

    // actions diverses
    bool attackAction(false);

    if(m_keys[m_actionKey])
    {
        for(Entity* gate : EntityManager::getEntities(GATE_TYPE))
        {
            gate->getComponent<GateComponent>()->enter();
        }
    }

    if(m_keys[m_attackKey])
    {
        attackAction = true;
    }

    // Résulat des actions
    bool moving(true);

    if(right == left)
    {
        movement->idle();
        sprite->setAction(m_lastDirection);
        moving = false;
    }

    if(attackAction)
    {
        if(m_owner->getComponent<AttackComponent>()->execute(m_lastDirection))
        {
            movement->idle();
            sprite->setAction(8 + m_lastDirection);
            moving = false;
        }
    }

    if(moving)
    {
        sprite->setAction(2 + m_lastDirection + 2 * sprint);
        movement->move(m_lastDirection, sprint);
    }
}

void KeyboardComponent::render()
{
}
