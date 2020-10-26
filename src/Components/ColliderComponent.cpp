#include "ColliderComponent.hpp"

ColliderComponent::ColliderComponent(Entity* owner, unsigned int w, unsigned int h) :
Component(owner),
m_falling(true),
m_spaceX(w),
m_spaceY(h),
m_redimensioned(true)
{
}

ColliderComponent::~ColliderComponent()
{
}

bool ColliderComponent::getFalling()
{
    return m_falling || m_owner->getComponent<TransformComponent>()->getVelocity(1) > 0;
}

void ColliderComponent::setFalling()
{
    m_falling = !m_falling;
}

bool ColliderComponent::AABB(SDL_Rect a, SDL_Rect b)
{
    if(a.x >= b.x + b.w)
    {
        return false;
    }
    if(a.x + a.w <= b.x)
    {
        return false;
    }
    if(a.y >= b.y + b.h)
    {
        return false;
    }
    if(a.y + a.h <= b.y)
    {
        return false;
    }

    return true;
}

SDL_Rect ColliderComponent::getHitbox()
{
    SDL_Rect rect(m_owner->getComponent<TransformComponent>()->getRect(0));

    if(m_redimensioned)
    {
        rect.x += m_spaceX*SCALE;
        rect.w -= 2*m_spaceX*SCALE;
        rect.y += m_spaceY*SCALE;
        rect.h -= 2*m_spaceY*SCALE;
    }

    return rect;
}

void ColliderComponent::update()
{
    TransformComponent* transform(m_owner->getComponent<TransformComponent>());

    m_oldRect = transform->getRect(0);
    m_newRect = transform->getRect(1);

    //delete transform;
    //transform = nullptr;

    if(m_redimensioned)
    {
        m_oldRect.x += m_spaceX*SCALE;
        m_oldRect.w -= 2*m_spaceX*SCALE;
        m_oldRect.y += m_spaceY*SCALE;
        m_oldRect.h -= 2*m_spaceY*SCALE;
        m_newRect.x += m_spaceX*SCALE;
        m_newRect.w -= 2*m_spaceX*SCALE;
        m_newRect.y += m_spaceY*SCALE;
        m_newRect.h -= 2*m_spaceY*SCALE;
    }

    collisionsDetection();
}

void ColliderComponent::render()
{
}

void ColliderComponent::setNeighbourBlocks()
{
    m_neighbourBlocks.clear();

    unsigned int x(m_newRect.x/(TILE_SIZE*SCALE));
    unsigned int y(m_newRect.y/(TILE_SIZE*SCALE));

    bool espaceW(m_newRect.w%(TILE_SIZE*SCALE) > m_newRect.w/(TILE_SIZE*SCALE));
    bool espaceH(m_newRect.h%(TILE_SIZE*SCALE) > m_newRect.h/(TILE_SIZE*SCALE));

    int w(m_newRect.w/(TILE_SIZE*SCALE) - !espaceW);
    int h(m_newRect.h/(TILE_SIZE*SCALE) - !espaceH);

    for(unsigned int i(x); i < (x+2+w); ++i)
    {
        for(unsigned int j(y); j < (y+2+h); ++j)
        {
            if(map.getTile(i, j)->getCollisionState())
            {
                m_neighbourBlocks.push_back(SDL_Rect{i*TILE_SIZE*SCALE, j*TILE_SIZE*SCALE, TILE_SIZE*SCALE, TILE_SIZE*SCALE});
            }
        }
    }
}

void ColliderComponent::collisionsDetection()
{
    setNeighbourBlocks();

    m_collideLater.clear();

    TransformComponent* transform(m_owner->getComponent<TransformComponent>());

    for(unsigned int block(0); block < m_neighbourBlocks.size(); block++)
    {
        if(AABB(m_neighbourBlocks[block], m_newRect))
        {

            setCorrectDistances(m_neighbourBlocks[block]);

            if(dx_correction == 0)
            {
                m_newRect.y += dy_correction;
                transform->setVelocity(0, 1);
            }
            else if(dy_correction == 0)
            {
                m_newRect.x += dx_correction;
                transform->setVelocity(0, 0);
            }
            else
            {
                m_collideLater.push_back(m_neighbourBlocks[block]);
            }
        }
    }

    for(unsigned int block(0); block < m_collideLater.size(); block++)
    {
        setCorrectDistances(m_neighbourBlocks[block]);

        if(((dx_correction == 0) && (dy_correction == 0)))
        {
            if(abs(dx_correction) < abs(dy_correction))
            {
                dy_correction = 0;
            }
            else if(abs(dy_correction) < abs(dx_correction))
            {
                dx_correction = 0;
            }
            if(dy_correction != 0)
            {
                m_newRect.y += dy_correction;
                transform->setVelocity(0, 1);
            }
            else if(dx_correction != 0)
            {
                m_newRect.x += dx_correction;
                transform->setVelocity(0, 0);
            }
        }
    }
    if(m_redimensioned)
    {
        m_newRect.x -= m_spaceX*SCALE;
        m_newRect.y -= m_spaceY*SCALE;
    }

    transform->setPosition(m_newRect.x, m_newRect.y);

    //delete transform;
    //transform = nullptr;
}

void ColliderComponent::setCorrectDistances(SDL_Rect block)
{
    dx_correction = 0;
    dy_correction = 0;

    // correction de la position y
    if((m_oldRect.y + m_oldRect.h <= block.y) && (block.y < m_newRect.y + m_newRect.h))
    {
        dy_correction = block.y - (m_newRect.y + m_newRect.h);
        m_falling = false;
    }
    else if((m_oldRect.y >= block.y + block.h) && (block.y + block.h > m_newRect.y))
    {
        dy_correction = (block.y + block.h) - m_newRect.y;
    }

    // correction de la position x
    if((m_oldRect.x + m_oldRect.w <= block.x) && (block.x < m_newRect.x + m_newRect.w))
    {
        dx_correction = block.x - (m_newRect.x + m_newRect.w);
    }
    else if((m_oldRect.x >= block.x + block.w) && (block.x + block.w > m_newRect.x))
    {
        dx_correction = (block.x + block.w) - m_newRect.x;
    }
}
