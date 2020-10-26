#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

#include <vector>
#include <utility>

#include "TransformComponent.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Managers/TextureManager.hpp"
#include "../Constants.hpp"

class SpriteComponent : public Component
{
    public:
        SpriteComponent(Entity* owner, TextureManager* texture, unsigned int width, unsigned int height, bool fixed, unsigned int speed, std::vector<std::pair<SpriteType, unsigned int> > frames);
        ~SpriteComponent();

        void update() override;
        void render() override;

        void setAction(unsigned int index);
        void setActivity(bool state);

        std::pair<unsigned int, unsigned int> getCurrentFrameIndex() const;
        bool getActivity() const;
        SpriteType getCurrentSpriteType() const;

    private:
        TextureManager* m_texture;
        SDL_Rect m_srcSize;
        SDL_Rect m_desSize;
        bool m_fixed;
        bool m_activity;
        unsigned int m_action;
        unsigned int m_start;
        unsigned int m_speed;
        std::vector<std::pair<SpriteType, unsigned int> > m_frames;
};

#endif
