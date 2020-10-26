#ifndef COLLIDERCOMPONENT_HPP
#define COLLIDERCOMPONENT_HPP

#include "TransformComponent.hpp"
#include "../Map.hpp"
#include <cmath>

extern Map map;

class ColliderComponent : public Component
{
    public:
        ColliderComponent(Entity* owner, unsigned int w, unsigned int h);
        ~ColliderComponent();

        void update() override;
        void render() override;

        bool getFalling();
        void setFalling();

        SDL_Rect getHitbox();

        static bool AABB(SDL_Rect a, SDL_Rect b);

    private:
        SDL_Rect m_oldRect;
        SDL_Rect m_newRect;
        std::vector<SDL_Rect> m_neighbourBlocks;
        std::vector<SDL_Rect> m_collideLater;
        int dx_correction;
        int dy_correction;
        bool m_falling;
        unsigned int m_spaceX;
        unsigned int m_spaceY;
        bool m_redimensioned;

        void setNeighbourBlocks();
        void collisionsDetection();
        void setCorrectDistances(SDL_Rect block);
};

#endif
