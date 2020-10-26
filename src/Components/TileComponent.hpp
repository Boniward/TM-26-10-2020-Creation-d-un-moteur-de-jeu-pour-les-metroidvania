#ifndef TILECOMPONENT_HPP
#define TILECOMPONENT_HPP

#include "../Component.hpp"
#include "../Managers/TextureManager.hpp"
#include "SDL.h"
#include <iostream>

extern AssetManager assetManager;

class TileComponent : public Component
{
    public:
        TileComponent(unsigned int x, unsigned int y, bool collision);
        TileComponent(unsigned int x, unsigned int y, bool collision, unsigned int textureId);
        ~TileComponent() override;

        void update() override;
        void render() override;

        bool getCollisionState() const;

    private:
        bool m_visible;
        bool m_collisionState;
        unsigned int m_mapPosition[2];
        TextureManager* m_texture;
        SDL_Rect m_srcRect;
        SDL_Rect m_desRect;
};

#endif
