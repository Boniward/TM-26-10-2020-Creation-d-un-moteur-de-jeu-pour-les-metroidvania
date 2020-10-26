#ifndef ATTACKCOMPONENT_HPP
#define ATTACKCOMPONENT_HPP

#include "TransformComponent.hpp"
#include "ColliderComponent.hpp"
#include "StatisticsComponent.hpp"
#include "InformationComponent.hpp"
#include <cmath>

class AttackComponent : public Component
{
    public:
        AttackComponent(Entity* owner, int x, int y, int w, int h, unsigned int frameAttack = 0, unsigned int cooldown = 0);
        ~AttackComponent();

        void update() override;
        void render() override;

        unsigned int getCooldown() const;
        ColliderComponent* getAttackBox(bool direction);
        unsigned int getFrameAttack() const;

        bool execute(bool direction = true);
        void attack(bool direction = true);

    private:
        Entity* m_attackBox;
        int m_spaceX;
        int m_spaceY;
        unsigned int m_lastAttack;
        unsigned int m_cooldown;
        unsigned int m_frameAttack;
};

#endif
