#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

#include "../Component.hpp"
#include "SDL.h"

class TransformComponent : public Component
{
    public:
        TransformComponent(Entity* owner, int x, int y, unsigned int w, unsigned int h, float vx, float vy, float ax, float ay);
        ~TransformComponent();

        void update() override;
        void render() override;

        void setPosition(int x, int y);
        void setVelocity(int velocity, bool index);

        int getPosition(bool index) const;
        int getVelocity(bool index) const;
        unsigned int getSize(bool index) const;
        SDL_Rect getRect(bool index) const; // 0 = old, 1 = new

    private:
        int m_position[2]; // x = 0, y = 1
        unsigned int m_size[2]; // w = 0, h = 1
        float m_velocity[2]; // x = 0, y = 1
        float m_acceleration[2]; // x = 0, y = 1
        static int m_accelerationLimit;
};

#endif
