#include "StatisticsComponent.hpp"

unsigned int StatisticsComponent::m_invincibilityTime(1000);

StatisticsComponent::StatisticsComponent(Entity* owner, int health, int mana, int damage) :
Component(owner),
m_maxHealth(health),
m_health(health),
m_maxMana(mana),
m_mana(mana),
m_damage(damage),
m_hit(false),
m_lastHit(0)
{
}

StatisticsComponent::StatisticsComponent(Entity* owner, int damage) :
Component(owner),
m_maxHealth(0),
m_health(0),
m_maxMana(0),
m_mana(0),
m_damage(damage),
m_hit(false),
m_lastHit(0)
{
}

StatisticsComponent::~StatisticsComponent()
{
}

void StatisticsComponent::update()
{
    if(SDL_GetTicks() - m_lastHit > m_invincibilityTime)
    {
        m_hit = false;
    }
}

void StatisticsComponent::render()
{
}

void StatisticsComponent::setHealth(int change)
{
    if(m_health + change < 0)
    {
        m_health = 0;
    }
    else if(m_maxHealth < m_health + change)
    {
        m_health = m_maxHealth;
    }
    else
    {
        m_health += change;
    }

    std::cout << m_owner->getName() << " : "<< m_health << "/" << m_maxHealth << " HP" << std::endl;
}

void StatisticsComponent::setMana(int change)
{
    if(m_mana + change < 0)
    {
        m_mana = 0;
    }
    else if(m_maxHealth < m_mana + change)
    {
        m_mana = m_maxMana;
    }
    else
    {
        m_mana += change;
    }

    std::cout << m_owner->getName() << " : "<< m_mana << "/" << m_maxMana << " MP" << std::endl;
}

void StatisticsComponent::setHit()
{
    m_hit = true;

    m_lastHit = SDL_GetTicks();
}

int StatisticsComponent::getHealth() const
{
    return m_health;
}

int StatisticsComponent::getMaxHealth() const
{
    return m_maxHealth;
}

int StatisticsComponent::getDamage() const
{
    return m_damage;
}

bool StatisticsComponent::getHit() const
{
    return m_hit;
}
