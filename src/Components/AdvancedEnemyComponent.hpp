#ifndef ADVANCEDENEMYCOMPONENT_HPP
#define ADVANCEDENEMYCOMPONENT_HPP

#include "AttackComponent.hpp"
#include "SpriteComponent.hpp"
#include "MovementComponent.hpp"

class AdvancedEnemyComponent : public Component
{
    public:
        AdvancedEnemyComponent(Entity* owner, int rangeX, int rangeY);
        ~AdvancedEnemyComponent();

        void update() override;
        void render() override;

        bool isPLayerInRange();

    private:
        int m_rangeX;
        int m_rangeY;
        unsigned int m_lastAttack;
        bool m_lastDirection;
};

#endif
