#ifndef BASICTRAPCOMPONENT_HPP
#define BASICTRAPCOMPONENT_HPP

#include "AttackComponent.hpp"

class BasicTrapComponent : public Component
{
    public:
        BasicTrapComponent(Entity* owner);
        ~BasicTrapComponent();

        void update() override;
        void render() override;
};

#endif
