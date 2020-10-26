#ifndef MOVEMENTCOMPONENT_HPP
#define MOVEMENTCOMPONENT_HPP

#include <cmath>

#include "TransformComponent.hpp"

class MovementComponent : public Component
{
    public:
        MovementComponent(Entity* owner, unsigned int speed);
        ~MovementComponent();

        void update() override;
        void render() override;

        void idle();
        void move(bool direction, bool sprint);
        void jump();
        void dodge(bool direction);

    private:
        unsigned int m_speed;
};

#endif
