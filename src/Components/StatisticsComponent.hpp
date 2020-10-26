#ifndef STATISTICSCOMPONENT_HPP
#define STATISTICSCOMPONENT_HPP

#include "../Component.hpp"
#include "SDL.h"
#include "../Game.hpp"

class StatisticsComponent : public Component
{
    public:
        StatisticsComponent(Entity* owner, int health, int mana, int damage);
        StatisticsComponent(Entity* owner, int damage);
        ~StatisticsComponent();

        void update() override;
        void render() override;

        void setHealth(int change);
        void setMana(int change);
        void setHit();

        int getHealth() const;
        int getMaxHealth() const;
        int getDamage() const;
        bool getHit() const;

    private:
        int m_health;
        int m_mana;
        int m_damage;
        int m_maxHealth;
        int m_maxMana;
        unsigned int m_lastHit;
        bool m_hit;

        static unsigned int m_invincibilityTime;
};

#endif
