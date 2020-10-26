#include "TileComponent.hpp"

TileComponent::TileComponent(unsigned int x, unsigned int y, bool collision) :
m_visible(true),
m_mapPosition{y, x},
m_collisionState(collision)
{
}

TileComponent::TileComponent(unsigned int x, unsigned int y, bool collision, unsigned int textureId) :
m_visible(false),
m_mapPosition{y, x},
m_collisionState(collision),
m_srcRect{32*(textureId%4), 32*((textureId-(textureId%4))/4), 32, 32},
m_desRect{m_mapPosition[0]*TILE_SIZE*SCALE, m_mapPosition[1]*TILE_SIZE*SCALE, TILE_SIZE*SCALE, TILE_SIZE*SCALE},
m_texture(assetManager.getTexture("map"))
{
}

TileComponent::~TileComponent()
{
    delete m_texture;
}

void TileComponent::update()
{
    if(!m_visible)
    {
        m_desRect = {m_mapPosition[0]*TILE_SIZE*SCALE, m_mapPosition[1]*TILE_SIZE*SCALE, TILE_SIZE*SCALE, TILE_SIZE*SCALE};
    }
}

void TileComponent::render()
{
    if(!m_visible)
    {
        m_texture->draw(&m_srcRect, &m_desRect, false);
    }
}

bool TileComponent::getCollisionState() const
{
    return m_collisionState;
}
