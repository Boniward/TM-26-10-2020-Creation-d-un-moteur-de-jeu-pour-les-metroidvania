#include "TransformComponent.hpp"

int TransformComponent::m_accelerationLimit((TILE_SIZE*SCALE)/2);

TransformComponent::TransformComponent(Entity* owner, int x, int y, unsigned int w, unsigned int h, float vx, float vy, float ax, float ay) :
Component(owner),
m_position{SCALE * x, SCALE * y},
m_size{SCALE * w, SCALE * h},
m_velocity{SCALE * vx, SCALE * vy},
m_acceleration{SCALE * ax, SCALE * ay}
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::update()
{
    // MRUA de l'objet sur l'axe x
    if(m_velocity[0] + m_acceleration[0] > m_accelerationLimit)
    {
        m_velocity[0] = m_accelerationLimit;
    }
    else if(m_velocity[0] + m_acceleration[0] < -m_accelerationLimit)
    {
        m_velocity[0] = -m_accelerationLimit ;
    }
    else
    {
        m_velocity[0] += m_acceleration[0];
    }

    // MRUA de l'objet sur l'axe y
    if(m_velocity[1] + m_acceleration[1] > m_accelerationLimit)
    {
        m_velocity[1] = m_accelerationLimit;
    }
    else if(m_velocity[1] + m_acceleration[1] < -m_accelerationLimit)
    {
        m_velocity[1] = -m_accelerationLimit;
    }
    else
    {
        m_velocity[1] += m_acceleration[1];
    }

}

void TransformComponent::render()
{
}

void TransformComponent::setPosition(int x, int y)
{
    m_position[0] = x;
    m_position[1] = y;
}

void TransformComponent::setVelocity(int velocity, bool index)
{
    m_velocity[index] = velocity;
}

int TransformComponent::getPosition(bool index) const
{
    if(m_position[index] < 0)
    {
        return 0;
    }
    return m_position[index];
}

int TransformComponent::getVelocity(bool index) const
{
    return m_velocity[index];
}

unsigned int TransformComponent::getSize(bool index) const
{
    return m_size[index];
}

SDL_Rect TransformComponent::getRect(bool index) const // 0 = old, 1 = new
{
    if(index)
    {
        return {m_position[0] + m_velocity[0], m_position[1] + m_velocity[1], m_size[0], m_size[1]};
    }

    return {m_position[0], m_position[1], m_size[0], m_size[1]};
}
