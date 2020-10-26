#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "Entity.hpp"
#include "Constants.hpp"
#include <iostream>

class Entity;

class Component
{
    public:
        Component();
        Component(Entity* owner);
        virtual ~Component();

        virtual void update() = 0;
        virtual void render() = 0;

    protected:
        Entity* m_owner;
};

#endif
