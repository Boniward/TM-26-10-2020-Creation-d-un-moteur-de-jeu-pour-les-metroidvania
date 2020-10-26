#ifndef BASICENEMYCOMPONENT_HPP
#define BASICENEMYCOMPONENT_HPP

#include "TransformComponent.hpp"
#include "ColliderComponent.hpp"
#include "SpriteComponent.hpp"
#include "AttackComponent.hpp"

class BasicEnemyComponent : public Component
{
    public:
        BasicEnemyComponent(Entity* owner, int start, int distance, int speed);
        ~BasicEnemyComponent();

        void update() override;
        void render() override;

    private:
        TransformComponent* m_transform;
        SpriteComponent* m_sprite;
        int m_roadA;
        int m_roadB;
        int m_speed;
};

#endif
